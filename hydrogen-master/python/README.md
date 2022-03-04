
# installation

\# https://stackoverflow.com/a/12806325
sudo apt-get install libbz2-dev

use pyenv to install 3.7.0

create a virtualenv "h2" in the current folder, install requirements and jupyter notebook.

``` bash

sudo apt-get update

# needed for jupyter notebook
sudo apt-get install libsqlite3-dev

# needed for

# needed for graph rendering
sudo apt install python-pydot python-pydot-ng graphviz

# pyenv installation
## https://github.com/pyenv/pyenv#installation
git clone https://github.com/pyenv/pyenv.git ~/.pyenv

echo 'export PYENV_ROOT="$HOME/.pyenv"' >> ~/.bashrc
echo 'export PATH="$PYENV_ROOT/bin:$PATH"' >> ~/.bashrc
echo -e 'if command -v pyenv 1>/dev/null 2>&1; then\n  eval "$(pyenv init -)"\nfi' >> ~/.bashrc

# ensure pyenv is present in the path
echo 'export PATH=$HOME/.pyenv/shims/python:$PATH' >> ~/.bashrc
echo 'export PATH=$HOME/.pyenv/bin/python:$PATH' >> ~/.bashrc

source ~/.bashrc

pyenv install 3.6.0
pyenv global 3.6.0
# ensure that ```pyenv versions``` points to 3.6.0
# ensure that ```which python3``` points to python3==3.6.0

pip3 install --user virtualenv 
virtualenv -p `which python3` h2
source h2/bin/activate
pip3 install -r requirements.txt
pip3 install notebook jupyter
```


# execution

``` bash
jupyter notebook
```
The above command opens a jupyter notebook session in your browser.


# profiling
cd ./python

## cProfile
PYTHONPATH=./h2py python3 -m cProfile -s tottime h2py/generation/certgenerator.py | less

## kernprof
PYTHONPATH=./h2py kernprof -l -v h2py/generation/certgenerator.py

## line_profiler
PYTHONPATH=./h2py kernprof -l h2py/generation/certgenerator.py
python3 -m line_profiler certgenerator.py.lprof



