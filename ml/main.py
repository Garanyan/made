import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.metrics import accuracy_score, log_loss
from nltk.corpus import wordnet
import string
from nltk import pos_tag
from nltk.corpus import stopwords
from nltk.tokenize import WhitespaceTokenizer
from nltk.stem import WordNetLemmatizer
from sklearn.ensemble import RandomForestClassifier
from sklearn.model_selection import train_test_split
from nltk.sentiment.vader import SentimentIntensityAnalyzer
from gensim.test.utils import common_texts
from sklearn.feature_extraction.text import CountVectorizer
from sklearn.feature_extraction.text import TfidfTransformer
from sklearn.linear_model import LogisticRegression, SGDClassifier
from sklearn.svm import LinearSVC
from sklearn.pipeline import Pipeline
from gensim.models.doc2vec import Doc2Vec, TaggedDocument
from sklearn.feature_extraction.text import TfidfVectorizer
import nltk
nltk.download('stopwords')
nltk.download('wordnet')
nltk.download('vader_lexicon')

pd.set_option('display.max_rows', 500)
pd.set_option('display.max_columns', 500)
pd.set_option('display.width', 1000)

reviews_df=pd.read_csv('products_sentiment_train.tsv',delimiter='\t',encoding='utf-8')
result_df=pd.read_csv('products_sentiment_test.tsv',delimiter='\t',encoding='utf-8')

print(list(reviews_df.columns.values)) #file header
print(reviews_df.head(5)) #last N rows


# train_texts, test_texts, y_train, y_test = \
#     train_test_split(category['sentence'], category['label'], test_size=0.2, random_state=42)
#
# print(category['label'].mean())
# mean_baseline = [category['label'].mean()] * len(y_test)
# print(log_loss(y_test, mean_baseline))

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


# clean text data
reviews_df["sentence_clean"] = reviews_df["sentence"].apply(lambda x: clean_text(x))
result_df["sentence_clean"] = reviews_df["sentence"].apply(lambda x: clean_text(x))
print(reviews_df.head(5))

sid = SentimentIntensityAnalyzer()
reviews_df["sentiments"] = reviews_df["sentence"].apply(lambda x: sid.polarity_scores(x))
reviews_df = pd.concat([reviews_df.drop(['sentiments'], axis=1), reviews_df['sentiments'].apply(pd.Series)], axis=1)

result_df["sentiments"] = result_df["sentence"].apply(lambda x: sid.polarity_scores(x))
result_df = pd.concat([result_df.drop(['sentiments'], axis=1), result_df['sentiments'].apply(pd.Series)], axis=1)

# add number of characters column
reviews_df["nb_chars"] = reviews_df["sentence"].apply(lambda x: len(x))
result_df["nb_chars"] = result_df["sentence"].apply(lambda x: len(x))

# add number of words column
reviews_df["nb_words"] = reviews_df["sentence"].apply(lambda x: len(x.split(" ")))
result_df["nb_words"] = result_df["sentence"].apply(lambda x: len(x.split(" ")))

documents = [TaggedDocument(doc, [i]) for i, doc in enumerate(reviews_df["sentence_clean"].apply(lambda x: x.split(" ")))]

# train a Doc2Vec model with our text data
model = Doc2Vec(documents, vector_size=5, window=2, min_count=1, workers=4)

# transform each document into a vector data
doc2vec_df = reviews_df["sentence_clean"].apply(lambda x: model.infer_vector(x.split(" "))).apply(pd.Series)
doc2vec_df.columns = ["doc2vec_vector_" + str(x) for x in doc2vec_df.columns]
reviews_df = pd.concat([reviews_df, doc2vec_df], axis=1)

doc2vec_df = result_df["sentence_clean"].apply(lambda x: model.infer_vector(x.split(" "))).apply(pd.Series)
doc2vec_df.columns = ["doc2vec_vector_" + str(x) for x in doc2vec_df.columns]
result_df = pd.concat([result_df, doc2vec_df], axis=1)

tfidf = TfidfVectorizer(min_df=10)
tfidf_result = tfidf.fit_transform(reviews_df["sentence_clean"]).toarray()
tfidf_df = pd.DataFrame(tfidf_result, columns=tfidf.get_feature_names())
tfidf_df.columns = ["word_" + str(x) for x in tfidf_df.columns]
tfidf_df.index = reviews_df.index
reviews_df = pd.concat([reviews_df, tfidf_df], axis=1)

tfidf_result_2 = tfidf.transform(result_df["sentence_clean"]).toarray()
tfidf_df_2 = pd.DataFrame(tfidf_result_2, columns=tfidf.get_feature_names())
tfidf_df_2.columns = ["word_" + str(x) for x in tfidf_df_2.columns]
tfidf_df_2.index = result_df.index
result_df = pd.concat([result_df, tfidf_df_2], axis=1)

print(reviews_df[reviews_df["nb_words"] >= 5].sort_values("pos", ascending = False)[["sentence", "pos"]].head(10))

label = "label"
ignore_cols = [label, "sentence", "sentence_clean"]
features = [c for c in reviews_df.columns if c not in ignore_cols]

X_train, X_test, y_train, y_test = train_test_split(reviews_df[features], reviews_df[label], test_size = 0.20, random_state = 42)

#rf = RandomForestClassifier(n_estimators = 100, random_state = 42)
rf = LogisticRegression(penalty='l2', random_state=42, max_iter=300, solver='liblinear')
rf.fit(reviews_df[features], reviews_df[label])

# show feature importance
# feature_importances_df = pd.DataFrame({"feature": features, "importance": rf.feature_importances_}).sort_values("importance", ascending = False)
# print(feature_importances_df.head(10))

test_probs = rf.predict_proba(X_test)
print(log_loss(y_test, test_probs))
print(log_loss(y_test, y_test + 0.1))

test_probs = rf.predict_proba(result_df[features])
pd.DataFrame(test_probs)[1].to_csv("file.csv")
# y = reviews_df['label']
# texts = reviews_df.drop(columns=['label', 'sentence'])
# train_texts, test_texts, y_train, y_test = \
#     train_test_split(texts,y, test_size=0.2, random_state=42)
#
# pipeline = Pipeline([
#     #('vec', CountVectorizer()),
#    # ('tfidf', TfidfTransformer()),
#     ('clf', LogisticRegression(penalty='l1')),
# ])
#
# pipeline.fit(train_texts, y_train)
# test_predictions = pipeline.predict(test_texts)
# test_probs = pipeline.predict_proba(test_texts)
# print(log_loss(y_test, test_probs))
# print(log_loss(y_test, y_test + 0.1))
# print(reviews_df.head(1))
