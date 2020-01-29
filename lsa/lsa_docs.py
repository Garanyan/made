# -*- coding: utf-8 -*-
"""
@author: Дмитрий

Модуль содержит класс Aggl, который реализует алгоритм агломеративной таксономии.
На вход алгоритма поступает матрица расстояний между кластеризуемыми объектами,
на выходе алгоритма получается список таксонов, каждый из которых сосоит из списка
индексов объектов, между которыми определена матрица расстояний.

Функция l2norm и класс Aggl добавлены сюда, чтобы не тянуть зависимости из проекта coreproto.
Они нужны только на этапе таксономии документов.
"""

import os
import json
import numpy as np
import codecs
import scipy.sparse as sparse
from sparsesvd import sparsesvd


def l2norm(vectors):
    """
    Нормирование векторов к единичной длине
    """
    if vectors.ndim == 1:
        l = np.linalg.norm(vectors)
        res = vectors / l
        return res
    l = np.linalg.norm(vectors, axis=1)
    res = vectors / l[:, None]
    return res


class Aggl(object):
    def __init__(self, distance_matrix, element_size=None, metric_type="Ward"):

        self.distances = np.asarray(distance_matrix, dtype=np.float64)
        self.metric = metric_type
        self.calculate_taxones_size = False

        if self.metric == "Ward":
            np.fill_diagonal(self.distances, 9999999)
        if self.metric == "wWard":
            np.fill_diagonal(self.distances, 9999999)
        if self.metric == "Avrg":
            np.fill_diagonal(self.distances, 9999999)
        if self.metric == "wAvrg":
            np.fill_diagonal(self.distances, 9999999)
        if self.metric == "Prob":
            np.fill_diagonal(self.distances, 0)

        if element_size is not None:
            self.calculate_taxones_size = True
            self.element_size = element_size

        # В случае использования меры близости Уорда, следует использовать
        # матрицу квадатов расстояний
        if self.metric == "Ward":
            self.distances = np.square(self.distances)
        if self.metric == "wWard":
            self.distances = np.square(self.distances)
        self.taxones = []
        self.md_list = []
        self.txn_size = []
        self.size = self.distances.shape[0]

        for i in range(self.size):
            taxone = [i]
            self.taxones.append(taxone)
            if self.calculate_taxones_size:
                self.txn_size.append(element_size[i])

    def aggl_cycle_dist(self):
        """
        Цикл агломеративной таксономии.
        """

        # 1. Определение индексов минимального значения в матрице расстояний
        indexes = np.unravel_index(np.argmin(self.distances), self.distances.shape)
        i_min = indexes[0]
        j_min = indexes[1]
        min_dist = self.distances[i_min, j_min]

        # 2. Перевычисление расстояния от нового таксона до оставшихся
        d_new = np.zeros(self.size)
        if self.metric == "Ward":
            for i in range(self.size):
                d_new[i] = (((len(self.taxones[i_min]) + len(self.taxones[i])) * self.distances[i_min, i] +
                             (len(self.taxones[j_min]) + len(self.taxones[i])) * self.distances[j_min, i] -
                             len(self.taxones[i]) * self.distances[i_min, j_min]) /
                            (len(self.taxones[i_min]) + len(self.taxones[i]) + len(self.taxones[j_min])))

        if self.metric == "wWard":
            for i in range(self.size):
                d_new[i] = (((self.txn_size[i_min] + self.txn_size[i]) * self.distances[i_min, i] +
                             (self.txn_size[j_min] + self.txn_size[i]) * self.distances[j_min, i] -
                             self.txn_size[i] * self.distances[i_min, j_min]) /
                            (self.txn_size[i_min] + self.txn_size[i] + self.txn_size[j_min]))

        if self.metric == "Avrg":
            for i in range(self.size):
                d_new[i] = ((len(self.taxones[i_min]) * self.distances[i_min, i] +
                             len(self.taxones[j_min]) * self.distances[j_min, i]) /
                            (len(self.taxones[i_min]) + len(self.taxones[j_min])))

        if self.metric == "wAvrg":
            for i in range(self.size):
                d_new[i] = ((self.txn_size[i_min] * self.distances[i_min, i] +
                             self.txn_size[j_min] * self.distances[j_min, i]) /
                            (self.txn_size[i_min] + self.txn_size[j_min]))

        # 3. Обновление значений расстояний в матрице
        for i in range(self.size):
            self.distances[i_min, i] = self.distances[i, i_min] = d_new[i]

            # 4. Удаление из матрицы ненужных после объединения строки и столбца
        ids = range(self.size)
        ids = np.delete(ids, j_min)
        self.distances = self.distances[ids, :]
        self.distances = self.distances[:, ids]

        # 5. Содержимое удаленного таксона переносится в новый таксон.
        self.taxones[i_min].extend(self.taxones[j_min])
        del self.taxones[j_min]
        self.size = self.distances.shape[0]

        # 6. Перевычисление размеров таксонов
        if self.calculate_taxones_size:
            self.txn_size[i_min] = self.txn_size[i_min] + self.txn_size[j_min]
            del self.txn_size[j_min]

        return min_dist

    def aggl_cycle_probs(self):
        """
        Цикл агломеративной таксономии.
        Вариант реализации для матрицы вероятностей принадлежности одному таксону.
        одному таксону.
        """

        # 1. Определение индексов максимального значения в матрице вероятностей
        indexes = np.unravel_index(np.argmax(self.distances), self.distances.shape)
        i_max = indexes[0]
        j_max = indexes[1]
        max_prob = self.distances[i_max, j_max]

        # 2. Перевычисление вероятностей для нового таксона
        self.distances = self.distances / max_prob
        d_new = np.zeros(self.size)
        if self.metric == "Prob":
            for i in range(self.size):
                d_new[i] = max_prob * self.distances[i_max][i] * self.distances[j_max][i]

        # 3. Обновление значений расстояний в матрице
        for i in range(self.size):
            self.distances[i_max, i] = self.distances[i, i_max] = d_new[i]

            # 4. Удаление из матрицы ненужных после объединения строки и столбца
        ids = range(self.size)
        ids = np.delete(ids, j_max)
        self.distances = self.distances[ids, :]
        self.distances = self.distances[:, ids]

        # 5. Содержимое удаленного таксона переносится в новый таксон.
        self.taxones[i_max].extend(self.taxones[j_max])
        del self.taxones[j_max]
        self.size = self.distances.shape[0]

        # 6. Перевычисление размеров таксонов
        if self.calculate_taxones_size:
            self.txn_size[i_max] = self.txn_size[i_max] + self.txn_size[j_max]
            del self.txn_size[j_max]

        return max_prob

    def counter(self, size_thr=0):
        """
        Возвращает количество таксонов, размер которых больше size_thr
        """
        c = 0
        for i in range(len(self.txn_size)):
            if self.txn_size[i] > size_thr:
                c += 1
        return c

    def get_auto_taxones_amount(self, max_taxones_amount=6):
        """
        Автоматическое определение наилучшего числа таксонов.
        Число таксонов определяется по максимальному относительному приросту расстояния между объединяемыми объектами.
        """

        while self.size > 1:
            self.md_list.append(self.aggl_cycle_dist())
        max_jump = 0
        c = 0
        for i in range(len(self.md_list) - max_taxones_amount + 1, len(self.md_list)):
            jump = self.md_list[i] / self.md_list[i - 1]
            if jump > max_jump:
                max_jump = jump
                c = len(self.md_list) - i + 1
        return c

    def get_taxones_amount_by_size(self, desired_txn_size=0):
        """
        Определение числа таксонов, при котором получается максимальное число
        таксонов не менее заданного размера.
        """
        desired_txn_count_max = 0
        taxones_count = self.size
        while self.size > 1:
            if self.metric == "Ward":
                self.md_list.append(self.aggl_cycle_dist())
            if self.metric == "wWard":
                self.md_list.append(self.aggl_cycle_dist())
            if self.metric == "Avrg":
                self.md_list.append(self.aggl_cycle_dist())
            if self.metric == "wAvrg":
                self.md_list.append(self.aggl_cycle_dist())
            if self.metric == "Prob":
                self.md_list.append(self.aggl_cycle_probs())
            desired_txn_count = self.counter(desired_txn_size)
            if desired_txn_count > desired_txn_count_max:
                desired_txn_count_max = desired_txn_count
                taxones_count = self.size
        return taxones_count, desired_txn_count_max

    def agglomeration(self, n=1):
        """
        Агломеративное объединение таксонов до заданного количества.
        """
        while self.size > n:
            print(f"Aglomerate: {self.size}")
            if self.metric == "Ward":
                self.md_list.append(self.aggl_cycle_dist())
            if self.metric == "wWard":
                self.md_list.append(self.aggl_cycle_dist())
            if self.metric == "Avrg":
                self.md_list.append(self.aggl_cycle_dist())
            if self.metric == "wAvrg":
                self.md_list.append(self.aggl_cycle_dist())
            if self.metric == "Prob":
                self.md_list.append(self.aggl_cycle_probs())
        return self.taxones


class LsaDocs(object):

    def __init__(self, terms_amount):
        """
        :param terms_amount: размер словаря термов
        """
        self.nterms = terms_amount
        self.row = None
        self.col = None
        self.data = None
        self.ut = None
        self.s = None
        self.vt = None
        self.doc_id = 0
        self.train_terms_amount = 0
        self.cidx = 0

    def distance_r2(self, v1, v2):
        return np.sum((v1 - v2) ** 2)

    def get_json_terms_count(self, path_to_json):
        """
        Вычисляет сумму всех употреблений термов во всех документах входного json-файла.
        """
        data = json.load(open(path_to_json))
        ndocs = len(data["docs"])
        nterms = 0
        for i in range(ndocs):
            nterms += len(data["docs"][i]["kw"])
        return nterms

    def reserve_train_memory(self, path_to_train_data):
        """
        Выделение памяти для представления всех документов в CSC-матрице.
        """
        files = os.listdir(path_to_train_data)
        input_files = filter(lambda x: x.endswith("json"), files)
        for j in range(len(input_files)):
            print("Processing file: %s" % input_files[j])
            path = path_to_train_data + "/" + input_files[j]
            self.train_terms_amount += self.get_json_terms_count(path)
        self.row = np.zeros(self.train_terms_amount, dtype=np.int32)
        self.col = np.zeros(self.train_terms_amount, dtype=np.int32)
        self.data = np.zeros(self.train_terms_amount, dtype=np.float32)
        print("Train terms amount = %i" % len(self.data))

    def add_docs_from_json(self, path_to_json):
        """
        Добавление документов из входного файла в CSC-матрицу.
        """
        data = json.load(open(path_to_json))
        ndocs = len(data["docs"])
        for i in range(ndocs):
            nterms = len(data["docs"][i]["kw"])
            crow = np.ones(nterms, dtype=np.int32) * self.doc_id
            ccol = np.zeros(nterms, dtype=np.int32)
            cdat = np.zeros(nterms, dtype=np.float32)
            for j in range(nterms):
                ccol[j] = int(data["docs"][i]["kw"][j]["id"])
                cdat[j] = float(data["docs"][i]["kw"][j]["weight"])

            self.row[self.cidx: self.cidx + nterms] = crow
            self.col[self.cidx: self.cidx + nterms] = ccol
            self.data[self.cidx: self.cidx + nterms] = cdat
            self.doc_id += 1
            self.cidx += nterms

    def lsa_train(self, path_to_train_data):
        """
        Вычисление SVD-разложения CSC-матрицы, содержащей векторные представления документов обучающего корпуса.
        На вход: путь к обучающему корпусу (коллекция json-файлов)
        """
        self.reserve_train_memory(path_to_train_data)
        files = os.listdir(path_to_train_data)
        input_files = filter(lambda x: x.endswith("json"), files)
        for j in range(len(input_files)):
            print("Processing file: %s" % input_files[j])
            path = path_to_train_data + "/" + input_files[j]
            self.add_docs_from_json(path)
            self.get_info()
        self.get_lsa_matrix()

    def save_lsa(self, path_to_save):
        f = open(path_to_save, 'wb')
        np.save(f, self.s)
        np.save(f, self.vt)
        f.close()

    def load_lsa(self, path_to_lsa):
        f = open(path_to_lsa, 'rb')
        self.s = np.load(f)
        self.vt = np.load(f)
        f.close()

    def get_info(self):
        print("Processed %i docs." % self.doc_id)
        print("Non-zero weights for all docs %i" % self.data.shape)

    def get_lsa_matrix(self, nfactors=400):
        csc_mat = sparse.csc_matrix((self.data, (self.row, self.col)), shape=(self.doc_id, self.nterms))
        self.ut, self.s, self.vt = sparsesvd(csc_mat, nfactors)  # do SVD, asking for 100 factors

    def reduce_dimension(self, input_vectors):
        ovecs = np.dot(self.vt, np.transpose(input_vectors))
        ovecs = np.transpose(ovecs)  # / self.s # приведение к сферической форме
        return ovecs

    def get_doc_vecs_from_json(self, path_to_json):

        nterms = self.get_json_terms_count(path_to_json)
        row = np.zeros(nterms, dtype=np.int32)
        col = np.zeros(nterms, dtype=np.int32)
        dat = np.zeros(nterms, dtype=np.float32)
        doc_id = 0
        cidx = 0

        data = json.load(open(path_to_json))
        ndocs = len(data["docs"])
        doc_titles = []

        for i in range(ndocs):
            nterms = len(data["docs"][i]["kw"])
            crow = np.ones(nterms, dtype=np.int32) * doc_id
            ccol = np.zeros(nterms, dtype=np.int32)
            cdat = np.zeros(nterms, dtype=np.float32)
            doc_titles.append(data["docs"][i]["title"])
            for j in range(nterms):
                ccol[j] = int(data["docs"][i]["kw"][j]["id"])
                cdat[j] = float(data["docs"][i]["kw"][j]["weight"])

            row[cidx: cidx + nterms] = crow
            col[cidx: cidx + nterms] = ccol
            dat[cidx: cidx + nterms] = cdat
            cidx += nterms
            doc_id += 1

        doc_vecs = sparse.csc_matrix((dat, (row, col)), shape=(doc_id, self.nterms)).toarray()
        return doc_vecs, doc_titles

    def get_lsa_doc_vecs_from_json(self, path_to_json):
        doc_vecs, titles = self.get_doc_vecs_from_json(path_to_json)
        lsa_doc_vecs = self.reduce_dimension(doc_vecs)
        return lsa_doc_vecs, titles

    def get_json_docs_taxones(self, path_to_json, numtxns, path_to_save_result):

        print("LSA-vectorizing...")
        lsa_vecs, doc_titles = self.get_lsa_doc_vecs_from_json(path_to_json)
        lsa_vecs = l2norm(lsa_vecs)
        lsa_vecs = lsa_vecs
        doc_titles = doc_titles

        print("Distance matrix calculation...")
        nvecs = lsa_vecs.shape[0]
        dist = np.zeros((nvecs, nvecs), dtype=np.float32)
        for i in range(nvecs):
            for j in range(nvecs):
                if i > j:
                    dist[i, j] = dist[j, i] = self.distance_r2(lsa_vecs[i], lsa_vecs[j])

        print("Agglomeration...")
        aggl_obj = Aggl(distance_matrix=dist)
        taxones = aggl_obj.agglomeration(n=numtxns)

        print("Saving result...")
        f = codecs.open(path_to_save_result, 'w', 'utf-8')
        n = 0
        for txn in taxones:
            s = 10 * "-" + " TAXON " + str(n) + " " + 10 * "-" + "\n"
            f.write(s)
            for i in range(len(txn)):
                s = doc_titles[txn[i]] + "\n"
                f.write(s)
            n += 1
        f.close()


if __name__ == "__main__":
    # original doc vector size
    terms_amount = 213887

    # path to LSA-matrix
    path_to_lsa = "/home/ovanes/asr/stlab/prototypes/lsa/data/model/kw_full.lsa"

    # init
    lsa_obj = LsaDocs(terms_amount)

    # ------ LSA train -------
    # path_to_train_data = r"d:\Data\Texts\rus\kw\full"
    # lsa_obj.lsa_train(path_to_train_data)
    # lsa_obj.save_lsa(path_to_lsa)

    # ------- LSA test -------
    path_to_json = "/home/ovanes/asr/stlab/prototypes/lsa/data/doc_terms/kw_3inews_common_ru_2017-09-01.json"
    path_to_result = "/home/ovanes/asr/stlab/prototypes/lsa/data/results/test_taxones.txt"
    lsa_obj.load_lsa(path_to_lsa)
    lsa_obj.get_json_docs_taxones(path_to_json, 100, path_to_result)
