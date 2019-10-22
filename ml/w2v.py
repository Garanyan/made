import tensorflow as tf
import numpy as np
from joblib import Parallel, delayed
from tqdm import tqdm
from gensim.models import FastText
import pandas as pd
from sklearn.metrics import accuracy_score, log_loss
from nltk.stem import WordNetLemmatizer
from nltk.corpus import wordnet
from nltk.corpus import stopwords
import string
from sklearn.model_selection import train_test_split
from nltk import pos_tag
import nltk
nltk.download('wordnet')
nltk.download('stopwords')


class Model(object):

    def __init__(self, fea_size):


        self.num_features = fea_size
        self.num_classes = 2  # +1 for CTC blank class

        self.graph = tf.Graph()
        with self.graph.as_default():

            # set placeholders
            self.inputs = tf.placeholder(tf.float32, [None, self.num_features], name="w2v")
            self.targets = tf.placeholder(tf.int32, [None, self.num_classes])
            self.learning_rate = tf.placeholder(tf.float32)


            # Truncated normal with mean 0 and stdev=0.1
            W_1 = tf.Variable(tf.truncated_normal([self.num_features, self.num_features], stddev=0.1), name="Weights_1")
            b_1 = tf.Variable(tf.constant(0., shape=[self.num_features]), name="Biases_1")

            dense_out = tf.nn.sigmoid(tf.matmul(self.inputs, W_1) + b_1)
            

            W_2 = tf.Variable(tf.truncated_normal([self.num_features, self.num_classes], stddev=0.1), name="Weights_2")
            b_2 = tf.Variable(tf.constant(0., shape=[self.num_classes]), name="Biases_2")

            self.logits = tf.matmul(dense_out, W_2) + b_2
            self.probs = tf.nn.softmax(self.logits, name="probs")

            self.loss = tf.reduce_mean(tf.nn.softmax_cross_entropy_with_logits_v2(logits=self.logits, labels=self.targets))
            correct_pred = tf.equal(tf.argmax(self.probs, axis=-1), tf.argmax(self.targets, axis=-1))
            self.accuracy = tf.reduce_mean(tf.cast(correct_pred, tf.float32))

            self.optimizer = tf.train.GradientDescentOptimizer(learning_rate=self.learning_rate)
            self.trainer = self.optimizer.minimize(self.loss)

            self.sess_init = tf.global_variables_initializer()

        config = tf.ConfigProto(allow_soft_placement=True, log_device_placement=False)
        config.gpu_options.visible_device_list = "0"  # only the gpu 0 is allowed
        config.gpu_options.per_process_gpu_memory_fraction = 0.4
        # config.gpu_options.per_process_gpu_memory_fraction = 0.4
        self.session = tf.Session(graph=self.graph, config=config)
        self.session.run(self.sess_init)
        #self.cost = tf.reduce_mean(self.loss)

    def save_model(self, best=False):
        with self.session.graph.as_default():

            variables_names = [v.name for v in tf.trainable_variables()]
            values = self.session.run(variables_names)

            if best:
                nn_cfg_file = open("cfg.best", mode="w")
                nn_weights_file = open("weights.bin", mode="wb")
            else:
                nn_cfg_file = open("cfg.best", mode="w")
                nn_weights_file = open("weights.bin", mode="wb")

            for names in variables_names:
                nn_cfg_file.write(names + "\n")

            for vals in values:
                np.save(nn_weights_file, vals)
            nn_weights_file.close()
            nn_cfg_file.close()

def get_wordnet_pos(pos_tag):
    if pos_tag.startswith('J'):
        return wordnet.ADJ
    elif pos_tag.startswith('V'):
        return wordnet.VERB
    elif pos_tag.startswith('N'):
        return wordnet.NOUN
    elif pos_tag.startswith('R'):
        return wordnet.ADV
    else:
        return wordnet.NOUN

def clean_text(text):
    # lower text
    text = text.lower()
    # tokenize text and remove puncutation
    text = [word.strip(string.punctuation) for word in text.split(" ")]
    # remove words that contain numbers
    text = [word for word in text if not any(c.isdigit() for c in word)]
    # remove stop words
    stop = stopwords.words('english')
    text = [x for x in text if x not in stop]
    # remove empty tokens
    text = [t for t in text if len(t) > 0]
    # pos tag text
    pos_tags = pos_tag(text)
    # lemmatize text
    text = [WordNetLemmatizer().lemmatize(t[0], get_wordnet_pos(t[1])) for t in pos_tags]
    # remove words with only one letter
    text = [t for t in text if len(t) > 1]
    # join all
    text = " ".join(text)
    return (text)



def get_one_hot_vectors(labels_sequence, num_classes):
    one_hot = np.zeros((len(labels_sequence), num_classes), dtype=np.int32)
    for i in range(len(labels_sequence)):
        one_hot[i, labels_sequence[i]] = 1
    return one_hot

def loading(line):
    values = line.rstrip().rsplit(' ')
    word = values[0]
    coefs = np.asarray(values[1:], dtype='float32')
    return word, coefs

if __name__ == "__main__":

    #fasttext = FastText.load_fasttext_format('/home/ovanes/Downloads/wiki.en.vec')
    #fasttext = FastText.load('wiki.en.vec')
    reviews_df = pd.read_csv('products_sentiment_train.tsv', delimiter='\t', encoding='utf-8')
    reviews_df["sentence_clean"] = reviews_df["sentence"].apply(lambda x: clean_text(x))

    result_df = pd.read_csv('products_sentiment_test.tsv', delimiter='\t', encoding='utf-8')
    result_df["sentence_clean"] = result_df["sentence"].apply(lambda x: clean_text(x))

    f = open('/home/ovanes/Downloads/wiki.en.vec', 'r', encoding='utf-8')

    embeddings_index = dict(Parallel(n_jobs=-1)(delayed(loading)(line) for line in tqdm(f)))
    f.close()

    X_train, X_test, y_train, y_test = train_test_split(reviews_df["sentence_clean"], reviews_df['label'], test_size=0.20, random_state=42)

    #print(len(embeddings_index))
    output_file = open('result.csv', 'w')
    #print(embeddings_index["good"])
    np.set_printoptions(suppress=True)
    model = Model(300)
    with model.session.graph.as_default():
        for _ in range(2):
            for index, record in reviews_df.iterrows():
                #print(record['sentence_clean'])
                #print(record['label'])
                words = record['sentence_clean'].split(" ")
                mean_vect = np.zeros(300)
                for word in words:
                    print(word)
                    embed_ = embeddings_index.get(word)
                    if embed_ is None:
                        continue
                    mean_vect = mean_vect + embed_

                mean_vect = mean_vect/len(words)

                if record['label'] == 1:
                    one_hot = (0, 1)
                else:
                    one_hot = (1, 0)

                feed = {
                        model.inputs: [mean_vect],
                        model.targets: [one_hot],
                        model.learning_rate: 0.05
                        }

                train_step = model.session.run([model.trainer], feed_dict=feed)

                #print("step ")

        log_probs_acc = []
        for index, record in reviews_df.iterrows():
            # print(record['sentence_clean'])
            # print(record['label'])
            words = record['sentence_clean'].split(" ")
            mean_vect = np.zeros(300)
            for word in words:
                print(word)
                embed_ = embeddings_index.get(word)
                if embed_ is None:
                    continue
                mean_vect = mean_vect + embed_

            mean_vect = mean_vect / len(words)

            feed = {
                model.inputs: [mean_vect]
            }

            probs = model.session.run([model.probs], feed_dict=feed)
            log_probs_acc.append(probs[0][0])

        print("log_los:")
        print(log_loss(reviews_df['label'], log_probs_acc))
        print(log_loss(reviews_df['label'], reviews_df['label'] + 0.1))

        for index, record in result_df.iterrows():
            words = record['sentence_clean'].split(" ")
            mean_vect = np.zeros(300)
            for word in words:
                embed_ = embeddings_index.get(word)
                if embed_ is None:
                    continue
                mean_vect = mean_vect + embed_

            mean_vect = mean_vect / len(words)
            feed = {
                model.inputs: [mean_vect]
            }

            probs = model.session.run([model.probs], feed_dict=feed)
            output_file.write("{},{}\n".format(record['Id'], probs[0][0][1]))
            #print(probs[0])