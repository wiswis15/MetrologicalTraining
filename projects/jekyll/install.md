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

# How to install Jekyll 3.x

Jekyll 3.x might not be available as part of a default Linux distribution. Moreover, Jekyll 3.x requires a recent 'Ruby' version, which is probably (also) missing.

The outlined steps enable both installs and are performed on an Ubuntu 15.10 live-system. 

## Install missing dependencies

```
sudo apt-get install git libssl-dev libreadline-dev zlib1g-dev
```
## Prepare the environment

### Enable a custom 'Ruby' enviroment

Download the 'Ruby' environment manager 'rbenv'
```
git clone https://github.com/rbenv/rbenv.git ~/.rbenv
```
Update your search path
```
echo 'export PATH="$HOME/.rbenv/bin:$PATH"' >> ~/.bashrc
```
Reload the Bash profile with the new settings
```
source ~/.bashrc
```
Alternatively, replace the current shell
```
exec $SHELL
```
However, it is better to close the terminal and open a new one.
``` 
exit
```
Followed by 
```
ctrl+alt+t
```
This is the safer approach because it does not preserve the shell variables. Next, initialise the 'Ruby' environment
```
echo 'eval "$(rbenv init -)"' >> ~/.bashrc
```
Again, restart the terminal for the Bash settings to take effect.

### Enable custom 'Ruby' builds to be installed

Download the 'rbenv-build' plugin
```
git clone https://github.com/rbenv/ruby-build.git ~/.rbenv/plugins/ruby-build
```
Update your search path
```
echo 'export PATH="$HOME/.rbenv/plugins/ruby-build/bin:$PATH"' >> ~/.bashrc
```
Once again, restart the terminal.

### Make new 'Ruby' packages automatically available

To complete the environment, a sort of 'Ruby' package manager is required. This  package manager automatically configures the environment for new packages, a.k.a 'gems'. Therefore, install the 'rbenv-gem-rehash' plugin.

```
git clone https://github.com/rbenv/rbenv-gem-rehash.git ~/.rbenv/plugins/rbenv-gem-rehash
```
## Install the required packages

### Install 'Ruby'
Install 'Ruby' version 2.2.3
```
rbenv install 2.2.3
```
Tell the system to use this version
```
rbenv global 2.2.3
```
Check the installed 'Ruby' version
```
ruby -v
```
Add the 'Ruby' install location to the search path. Be aware that it is added at the end! Otherwise the enviroment has to be set to use the correct 'global' version for each new terminal (shell) session.
```
echo 'export PATH="$PATH:$HOME/.rbenv/bin"' >> ~/.bashrc
```
One more time to restart the terminal with the updated Bash settings before Jekyll is installed.

### Install the 'Ruby gems'

Add the 'gem' install location to the search path. In constrast to 'Ruby's' version its subpath contains the value '2.2.0'.
```
export 'export PATH="$HOME/.gem/ruby/2.2.0/bin:$PATH"' >> ~/.bashrc
```
Install the 'Jekyll gem' in the user's home directory after restarting our terminal.
```
gem install jekyll --user-install
```
Additional 'gems' can be installed in the same manner.
