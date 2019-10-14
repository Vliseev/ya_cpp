mkdir -p pvs
cd pvs || exit
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=On ../
pvs-studio-analyzer analyze -o $(pwd)/pvs_log.log -e $(pwd)
plog-converter -a GA:1,2 -t fullhtml -o $(pwd)/human_log.html $(pwd)/pvs_log.log
