
git hash: 8162b32c82c08d5ecbc64898e845af0625d88384

# output of make -j7 welcome; THREADS=6 OPTION=2 START=3 STOP=7 ./welcome

total for 3 nodes: 4 graphs in 0.00051863 seconds    
total for 4 nodes: 38 graphs in 0.00148806 seconds  
total for 5 nodes: 728 graphs in 0.0536038 seconds  
total for 6 nodes: 26704 graphs in 3.43966 seconds  
total for 7 nodes: 1866256 graphs in 376.749 seconds  


# output of certgenerator.py

h2_3.6.4 ❯ time PYTHONPATH=./h2py python3 h2py/generation/certgenerator.py  

all graphs: 3  
join  
n: 3 count: 2  
all graphs: 14  
join  
n: 4 count: 6  
all graphs: 90  
join  
n: 5 count: 21  
all graphs: 651  
join  
n: 6 count: 112  
all graphs: 7056  
join  
n: 7 count: 853  
num graphs generated: 995  
{2: 1, 3: 2, 4: 6, 5: 21, 6: 112, 7: 853}  
PYTHONPATH=./h2py python3 h2py/generation/certgenerator.py  40.60s user 0.54s system 214% cpu 19.153 total  


# output: 2018, aug 05

## 7 vertices
all graphs: 3  ``
trimmed from 3 to 2  
all graphs: 14  
trimmed from 14 to 6  
all graphs: 90  
trimmed from 90 to 21  
all graphs: 651  
trimmed from 651 to 112  
all graphs: 7056  
trimmed from 7056 to 853  
num graphs generated: 5  
PYTHONPATH=./h2py python3 h2py/generation/certgenerator.py -n 7  19.81s user 0.46s system 102% cpu 19.726 total  

## 8 vertices
```bash
time PYTHONPATH=./h2py python3 h2py/generation/certgenerator.py -n 8
```
all graphs: 3  
trimmed from 3 to 2  
all graphs: 14  
trimmed from 14 to 6  
all graphs: 90  
trimmed from 90 to 21  
all graphs: 651  
trimmed from 651 to 112  
all graphs: 7056  
trimmed from 7056 to 853  
all graphs: 108331  
trimmed from 108331 to 11121  
num graphs generated: 6  
PYTHONPATH=./h2py python3 h2py/generation/certgenerator.py -n 8  273.62s user 1.58s system 100% cpu 4:34.66 total  



# discussion

* the python module uses cert1, not cert2. if we implement and hence use cert2, the timings would be much better.
* its python code, with no cython. need to say anything?
* its python code. so c code would be way faster.


