[comment]: # (title: ...)
[comment]: # (author: ...)
[comment]: # (- name: ...)
[comment]: # (  affiliation: ...)
[comment]: # (  email: ...)
[comment]: # (- name: ...)
[comment]: # (  affiliation: ...)
[comment]: # (  email: ...)
[comment]: # ( date: ...)
[comment]: # ( abstract: ...)
[comment]: # ( bibliography: ...)

# How to install MkDocs

MkDocs might not be available as part of a default Linux distribution. Moreover, it requires a recent Python version. Versions 2.6, 2.7, 3.3, 3.4 and 3.5 are supported. A custom Python version might be preferred.

The outlined steps enable both installs and are performed on an Ubuntu 15.10 live-system. 

## Install missing dependencies

```
sudo apt-get install git make build-essential libssl-dev zlib1g-dev libbz2-dev libreadline-dev libsqlite3-dev wget curl llvm libncurses5-dev

```
## Prepare the environment

### Enable a custom Python enviroment

Download the Python environment manager 'pyenv'
```
git clone https://github.com/yyuu/pyenv ~/.pyenv
```
Define `PYENV_ROOT` and add it to the Bash profile
```
echo 'export PYENV_ROOT="$HOME/.pyenv"' >> ~/.bashrc
```
Close the terminal and open a new one.
``` 
exit
```
Followed by 
```
ctrl+alt+t
```
This is the safer approach because it does not preserve the shell variables. Next, add `PYENV_ROOT` to the search path.
```
echo 'export PATH="$PYENV_ROOT/bin:$PATH"' >> ~/.bashrc
```
Again, close the terminal and open a new one so we can initialise the Python environment.
```
echo 'eval "$(pyenv init -)"' >> ~/.bashrc
```
Once again, restart the terminal for the Bash settings to take effect.

The Python version and install location are controlled by `PENV_VERSION` and `PENV_ROOT`, respectively. Their values can be obtained by the commands

```
penv shell
penv root
```
## Install the required packages

### Install Python
Install Python version 3.5.1 in `$PYENV_ROOT/version`
```
pyenv install 3.5.1
```
Tell the system to use this version by setting `PYENV_VERSION`
```
export PYENV_VERSION=3.5.1
```
Check the installed Python version
```
python -V
```
###Install the Python packages 
Install the MkDocs package
```
pip install mkdocs
```
### Install MarkDown extenstions
Markdownextentions are hosted at [Python Markdown](https://pythonhosted.org/Markdown/extensions/) of which many are available in the installed MkDocs setup. Additional extensions are also provided by third-parties. Follow the individual installation instructions for these third-party extensions.
