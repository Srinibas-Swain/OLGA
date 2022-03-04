hydrogen
======================

# Installation

```bash
sudo apt-get install git 
sudo apt-get install libeigen3-dev clang libomp-dev
sudo apt-get install libreadline-gplv2-dev libncursesw5-dev libssl-dev libsqlite3-dev tk-dev libgdbm-dev libc6-dev libbz2-dev
```

## 1. for googletest
```bash
make init
```

## 2. for backward-cpp
```bash
git submodule init
git submodule update
```

## 3. backward-cpp dependencies
```bash
sudo apt-get -y install binutils-dev
```

## 4. create data folder

``` bash
mkdir data
```

## 4. finishing up
```bash
make clean
make welcome
THREADS=1 OPTION=3 START=3 STOP=4 ./welcome 
```


How to form the graph id
========================
1. Adj Matrix has three types of entries
  1. **#** form the lower triangle and you can ignore them. 
  * **$** are the diagonal entries and you can ignore them
  * {0,1} signify absense/presence of edge
1. Form the binary string by taking the cols of the adj matrix (just the upper triangular part) from the second col to the last col. 
  * Binary String = *second col* + ... + *last col*
1. Convert this to decimal and you have the graph id.

Examples
---------
Say your graph is a star of 5 vertices with the following adj matrix  

| A | 0 | 1 | 2 | 3 | 4 |  
|---|---|---|---|---|---|  
| 0 | $ | 1 | 1 | 1 | 0 |  
| 0 | $ | 1 | 1 | 1 | 0 |  
| 1 | # | $ | 1 | 0 | 0 |  
| 2 | # | # | $ | 0 | 0 |  
| 3 | # | # | # | $ | 0 |  
| 4 | # | # | # | # | $ |  


* In this case, the binary string is 1111000000 and the graph id is 512+256+128+64 = 960

2. For a cycle of length 4, adj matrix is
  * Adj Matrix  

| ^ | 0 | 1 | 2 | 3 |  
|---|---|---|---|---|  
| 0 | $ | 1 | 1 | 0 |  
| 1 | # | $ | 1 | 0 |  
| 2 | # | # | $ | 1 |  
| 3 | # | # | # | $ |  

  * Binary String = 111001 and graph id is 32+16+8+1 = 57

todo:
--------------
1. git lab?
