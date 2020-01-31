# configure environment

create requirements.txt:
```
jupyter
pylint
pytest
pytest-cov
pyyaml
```

then, create environment
```bash
export env_name="mail-py-2019-q4"
conda create -n $env_name python=3
conda activate $env_name
conda install --file requirements.txt
```

# scripts usage
```bash
pylint --output-format=colorized -d C0111,C0103 -v inverted_index.py
pytest -v .
pytest --cov -v .
pytest --cov -vv --durations=0 .
```

# download dataset
Wikipedia sample (zip) is available via the following link:
- https://drive.google.com/open?id=1ASO-nWW5FpvM7PfpOxxPu-0imjcMZhqN

Stop words:
- https://drive.google.com/open?id=1NBPhZzUyFc0e-_vQwZpxtrxBqzCsB9Yg
