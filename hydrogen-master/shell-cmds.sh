ag "x5.*anmly" method1.out | cut -d":" -f1 > method1.lines && ag "x5.*a.*[\d]+" method2.out | cut -d":" -f1 > method2.lines && diff method1.lines method2.lines | ag \> | cut -d " " -f2 | xargs -I l sh -c "sed -n l,lp\;lq method1.out; sed -n l,lp\;lq method2.out; echo"

