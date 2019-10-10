mkdir -p pvs
cd pvs || exit
cmake3 -DCMAKE_EXPORT_COMPILE_COMMANDS=On ../
pvs-studio-analyzer analyze -o $(pwd)/pvs_log.log -e $(pwd)
plog-converter -a GA:1,2 -t tasklist -o $(pwd)/human_log.log $(pwd)/pvs_log.log
