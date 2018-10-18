#!/bin/bash
function usage(){
   echo "usage: ./filter.sh <dir> "
}
if [ $# -le 0 ];then
   usage
   exit 0;
fi
echo $1
for file in $1/*; do
    if echo  $file |grep "pm_data";then
        echo "data file: $file"
        grep "MYST" $file >> $1/time.data
        grep "aosFwApl memory" $file |cut -d',' -f 2|cut -d':' -f 2|cut -d' ' -f 2 >> $1/apl_mem.data
        grep "aosFwHal memory" $file |cut -d',' -f 2|cut -d':' -f 2|cut -d' ' -f 2 >> $1/hal_mem.data
        grep "aosCorePmCol memory" $file |cut -d',' -f 2|cut -d':' -f 2|cut -d' ' -f 2 >> $1/pmc_mem.data
        grep "aosCorePmSrv memory" $file |cut -d',' -f 2|cut -d':' -f 2|cut -d' ' -f 2 >> $1/pms_mem.data
        grep "bin/aosFwApl" $file |grep -v PSS | awk -F' ' '{print $3}'  >> $1/apl_cpu.data
        grep "bin/aosFwHal" $file |grep -v PSS | awk -F' ' '{print $3}'  >> $1/hal_cpu.data
        grep "bin/aosCorePmCol" $file |grep -v PSS | awk -F' ' '{print $3}'  >> $1/pmc_cpu.data
        grep "bin/aosCorePmSrv" $file |grep -v PSS | awk -F' ' '{print $3}'  >> $1/pms_cpu.data
        grep "bin/aosDomTwamp" $file |grep -v PSS | awk -F' ' '{print $3}'  >> $1/twamp_cpu.data
        grep "hist" $file |grep -v disthist|tr -d '[a-zA-Z]'|awk -F' ' '{print $1}' >> $1/hist_pm_db.data
        grep "/var/opt/adva/aos/db/aplfw"  $file |grep  -v current |grep -v defaults|tr -d '[a-zA-Z]'|awk -F' ' '{print $1}' >> $1/apl_db.data
        grep "Amount of MO" $file >> $1/dte_apl_mo.data
        grep "persistent" $file >> $1/dte_apl_per_mo.data
        grep "Number of bins" $file >> $1/dte_pm_db.data
        grep "eidTree" $file |cut -d':' -f2|cut -d',' -f1 >> $1/dte_eidTreeNode.data
        grep Pss -i $file |grep Dom|head -n 15  >> $1/DomApp_init_mem.data
        grep Pss -i $file |grep Dom|tail -n 15  >> $1/DomApp_end_mem.data
        grep Pss -i $file |grep -v "Dom" |head -n 20  >> $1/CoreApp_init_mem.data
        grep Pss -i $file |grep -v "Dom" |tail -n 20 >> $1/CoreApp_end_mem.data
        grep "KiB Mem" $file |head  -n 1 >> $1/sys_init_mem.data
        grep "KiB Mem" $file |tail  -n 1 >> $1/sys_end_mem.data
        grep "Filesystem"  $file  -A 35|head -n 36 >> $1/flash_init.data
        grep "Filesystem"  $file  -A 35|tail -n 36 >> $1/flash_end.data
    fi
done
