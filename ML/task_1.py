import pandas as pd
import numpy as np
from nltk import ngrams
import nltk
from sklearn.feature_extraction.text import CountVectorizer
from sklearn.feature_extraction.text import TfidfTransformer
from sklearn.feature_extraction.text import TfidfVectorizer
from sklearn.naive_bayes import MultinomialNB
from sklearn.ensemble import RandomForestClassifier
from sklearn.linear_model import LogisticRegression, SGDClassifier
from sklearn.svm import LinearSVC
import re
from sklearn.metrics import accuracy_score, log_loss
from sklearn.pipeline import Pipeline
nltk.download('stopwords')
from nltk.tokenize import word_tokenize
from nltk.stem.porter import PorterStemmer
from nltk.corpus import stopwords
from sklearn.model_selection import train_test_split
from sklearn.metrics import classification_report, confusion_matrix, accuracy_score
# Read data from file 'filename.csv'
# (in the same directory that your python process is based)
# Control delimiters, rows, column names with read_csv (see later)
# data = pd.read_csv("products_sentiment_test.tsv")
data = pd.read_csv('products_sentiment_train.tsv', delimiter='\t', encoding='utf-8')
result_data = pd.read_csv('products_sentiment_test.tsv', delimiter='\t', encoding='utf-8')
print(data.columns.values)  # file header
text = data.loc[data['label'] == 1]['sentence']
porter = PorterStemmer()
stop_words = set(stopwords.words('english'))
whole_1_text = ""
new_sentensec = []
for sentense in data['sentence']:
    # print(sentense)
    tokens = word_tokenize(sentense)
    tokens = [w.lower() for w in tokens]
    words = [w for w in tokens if not w in stop_words]
    stemmed = [porter.stem(word) for word in tokens]

    new_sentence = " ".join(stemmed)
    # Remove single characters from the start
    new_sentence = re.sub(r'\^[a-zA-Z]\s+', ' ', new_sentence)

    # Substituting multiple spaces with single space
    new_sentence = re.sub(r'\s+', ' ', new_sentence, flags=re.I)

    new_sentensec.append(new_sentence.replace(".","").replace(",","").replace(")","").replace("(","").replace("0","").replace("1",""))
    whole_1_text = whole_1_text + "\n" + new_sentence
    # print(*twograms)

twograms = ngrams(whole_1_text.replace(".","").replace(",","").replace(")","").replace("(","").replace("0","").replace("1","").split(), 1)
fdist = nltk.FreqDist(twograms)
for k, v in sorted(fdist.items(), key = lambda x : x[1]):
    print(k, v)
print(text)
#
# Preview the first 5 lines of the loaded data


train_texts, test_texts, y_train, y_test = train_test_split(data['sentence'][1:], data['label'][1:], test_size=0.2, random_state=42)
pipeline = Pipeline([
    ('vec', CountVectorizer(ngram_range=(1, 1), min_df=5, max_df=0.9, tokenizer=nltk.word_tokenize, binary=True)),
    ('tfidf', TfidfTransformer()),
    ('clf', LogisticRegression(penalty='l2',max_iter=200, solver='liblinear')),
   # ('clf', SGDClassifier(tol=1e-3)),
    #('clf',  MultinomialNB())
])
pipeline.fit(data['sentence'][1:], data['label'][1:])
test_predictions = pipeline.predict(test_texts)
test_probs = pipeline.predict_proba(test_texts)
print(log_loss(y_test, test_probs))

predictions = pipeline.predict_proba(result_data['text'])
pd.DataFrame(predictions)[1].to_csv("file_5.csv")
#
# vectorizer = TfidfVectorizer (max_features=2500, min_df=7, max_df=0.8, stop_words=stopwords.words('english'))
#
#
# vectorizer.fit(data['sentence'][1:])
# vectorizer.fit(result_data['text'])
# processed_features =vectorizer.transform(data['sentence'][1:]).toarray()
# X_train, X_test, y_train, y_test = train_test_split(processed_features, data['label'][1:], test_size=0.1, random_state=1)
# text_classifier = RandomForestClassifier(n_estimators=200, random_state=0)
# text_classifier.fit(X_train, y_train)
# predictions = text_classifier.predict_proba(X_test)
#
# # print(confusion_matrix(y_test,predictions))
# # print(classification_report(y_test,predictions))
# # print(accuracy_score(y_test, predictions))
# print(log_loss(y_test, predictions))
# #print(data.head(5))  # last N rows
# #print(data[0:3])  # last N rows
#
# print(result_data.head())
# processed_features_result = vectorizer.transform(result_data['text']).toarray()
# predictions = text_classifier.predict_proba(processed_features_result)
# #np.savetxt("foo.csv", predictions, delimiter=",")
# pd.DataFrame(predictions).to_csv("file.csv")
#predictions.to_csv('results.csv', header=True)
