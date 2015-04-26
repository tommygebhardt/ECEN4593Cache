#!/bin/bash
# This script runs the cache simulations on each of the production trace
# files, storing the results in separate .dat files.


SECT1="# Cache Configuration File
# Keep order the same. All comments must be at beginning of file,
# preceeded by a '#' sign. The order should be: (##) = number, (';') is new line
# L1_block_size,##;L1_cache_size,##;L1_assoc, ##;L1_hit_time,##;
# L1_miss_time,##;L2_block_size,##;L2_cache_size,##;L2_assoc,##;
# L2_hit_time,##;L2_miss_time,##;L2_transfer_time,##;L2_bus_width,##;
L1_block_size,32\n"
L1_SIZE="L1_cache_size,8192\n"
L1_ASSOC="L1_assoc,1\n"
SECT2="L1_hit_time,1
L1_miss_time,1
L2_block_size,64\n"
L2_SIZE="L2_cache_size,32768\n"
L2_ASSOC="L2_assoc,1\n"
SECT3="L2_hit_time,5
L2_miss_time,7
L2_transfer_time,5
L2_bus_width,16"

# Default configuration
echo "Configuring memory system for Default..."
echo -e "$SECT1$L1_SIZE$L1_ASSOC$SECT2$L2_SIZE$L2_ASSOC$SECT3" > config.csv

echo "Starting bzip2 simulation"
zcat ../traces-long/bzip2.gz | ./main config.csv > ./output/bzip2_Default.dat

echo "Starting h264ref simulation"
zcat ../traces-long/h264ref.gz | ./main config.csv > ./output/h264ref_Default.dat

echo "Starting libquantum simulation"
zcat ../traces-long/libquantum.gz | ./main config.csv > ./output/limquantum_Default.dat

echo "Starting omnetpp simulation"
zcat ../traces-long/omnetpp.gz | ./main config.csv > ./output/omnetpp_Default.dat

echo "Starting sjeng simulation"
zcat ../traces-long/sjeng.gz | ./main config.csv > ./output/sjeng_Default.dat

# L1-2way configuration
echo "Configuring memory system for L1-2way..."
L1_ASSOC="L1_assoc,2\n"
echo -e "$SECT1$L1_SIZE$L1_ASSOC$SECT2$L2_SIZE$L2_ASSOC$SECT3" > config.csv

echo "Starting bzip2 simulation"
zcat ../traces-long/bzip2.gz | ./main config.csv > ./output/bzip2_L1-2way.dat

echo "Starting h264ref simulation"
zcat ../traces-long/h264ref.gz | ./main config.csv > ./output/h264ref_L1-2way.dat

echo "Starting libquantum simulation"
zcat ../traces-long/libquantum.gz | ./main config.csv > ./output/limquantum_L1-2way.dat

echo "Starting omnetpp simulation"
zcat ../traces-long/omnetpp.gz | ./main config.csv > ./output/omnetpp_L1-2way.dat

echo "Starting sjeng simulation"
zcat ../traces-long/sjeng.gz | ./main config.csv > ./output/sjeng_L1-2way.dat

# All-2way configuration
echo "Configuring memory system for All-2way..."
L2_ASSOC="L2_assoc,2\n"
echo -e "$SECT1$L1_SIZE$L1_ASSOC$SECT2$L2_SIZE$L2_ASSOC$SECT3" > config.csv

echo "Starting bzip2 simulation"
zcat ../traces-long/bzip2.gz | ./main config.csv > ./output/bzip2_All-2way.dat

echo "Starting h264ref simulation"
zcat ../traces-long/h264ref.gz | ./main config.csv > ./output/h264ref_All-2way.dat

echo "Starting libquantum simulation"
zcat ../traces-long/libquantum.gz | ./main config.csv > ./output/limquantum_All-2way.dat

echo "Starting omnetpp simulation"
zcat ../traces-long/omnetpp.gz | ./main config.csv > ./output/omnetpp_All-2way.dat

echo "Starting sjeng simulation"
zcat ../traces-long/sjeng.gz | ./main config.csv > ./output/sjeng_All-2way.dat


# L2-4way configuration
echo "Configuring memory system for L2-4way..."
L2_ASSOC="L2_assoc,4\n"
echo -e "$SECT1$L1_SIZE$L1_ASSOC$SECT2$L2_SIZE$L2_ASSOC$SECT3" > config.csv

echo "Starting bzip2 simulation"
zcat ../traces-long/bzip2.gz | ./main config.csv > ./output/bzip2_L2-4way.dat

echo "Starting h264ref simulation"
zcat ../traces-long/h264ref.gz | ./main config.csv > ./output/h264ref_L2-4way.dat

echo "Starting libquantum simulation"
zcat ../traces-long/libquantum.gz | ./main config.csv > ./output/limquantum_L2-4way.dat

echo "Starting omnetpp simulation"
zcat ../traces-long/omnetpp.gz | ./main config.csv > ./output/omnetpp_L2-4way.dat

echo "Starting sjeng simulation"
zcat ../traces-long/sjeng.gz | ./main config.csv > ./output/sjeng_L2-4way.dat


# All-4way configuration
echo "Configuring memory system for All-4way..."
L1_ASSOC="L1_assoc,4\n"
echo -e "$SECT1$L1_SIZE$L1_ASSOC$SECT2$L2_SIZE$L2_ASSOC$SECT3" > config.csv

echo "Starting bzip2 simulation"
zcat ../traces-long/bzip2.gz | ./main config.csv > ./output/bzip2_All-4way.dat

echo "Starting h264ref simulation"
zcat ../traces-long/h264ref.gz | ./main config.csv > ./output/h264ref_All-4way.dat

echo "Starting libquantum simulation"
zcat ../traces-long/libquantum.gz | ./main config.csv > ./output/limquantum_All-4way.dat

echo "Starting omnetpp simulation"
zcat ../traces-long/omnetpp.gz | ./main config.csv > ./output/omnetpp_All-4way.dat

echo "Starting sjeng simulation"
zcat ../traces-long/sjeng.gz | ./main config.csv > ./output/sjeng_All-4way.dat


# L1-8way configuration
echo "Configuring memory system for L1-8way..."
L1_ASSOC="L1_assoc,8\n"
L2_ASSOC="L2_assoc,1\n"
echo -e "$SECT1$L1_SIZE$L1_ASSOC$SECT2$L2_SIZE$L2_ASSOC$SECT3" > config.csv

echo "Starting bzip2 simulation"
zcat ../traces-long/bzip2.gz | ./main config.csv > ./output/bzip2_L1-8way.dat

echo "Starting h264ref simulation"
zcat ../traces-long/h264ref.gz | ./main config.csv > ./output/h264ref_L1-8way.dat

echo "Starting libquantum simulation"
zcat ../traces-long/libquantum.gz | ./main config.csv > ./output/limquantum_L1-8way.dat

echo "Starting omnetpp simulation"
zcat ../traces-long/omnetpp.gz | ./main config.csv > ./output/omnetpp_L1-8way.dat

echo "Starting sjeng simulation"
zcat ../traces-long/sjeng.gz | ./main config.csv > ./output/sjeng_L1-8way.dat


# L2-Big configuration
echo "Configuring memory system for L2-Big..."
L1_ASSOC="L1_assoc,2\n"
L2_SIZE="L2_cache_size,65536\n"
echo -e "$SECT1$L1_SIZE$L1_ASSOC$SECT2$L2_SIZE$L2_ASSOC$SECT3" > config.csv

echo "Starting bzip2 simulation"
zcat ../traces-long/bzip2.gz | ./main config.csv > ./output/bzip2_L2-Big.dat

echo "Starting h264ref simulation"
zcat ../traces-long/h264ref.gz | ./main config.csv > ./output/h264ref_L2-Big.dat

echo "Starting libquantum simulation"
zcat ../traces-long/libquantum.gz | ./main config.csv > ./output/limquantum_L2-Big.dat

echo "Starting omnetpp simulation"
zcat ../traces-long/omnetpp.gz | ./main config.csv > ./output/omnetpp_L2-Big.dat

echo "Starting sjeng simulation"
zcat ../traces-long/sjeng.gz | ./main config.csv > ./output/sjeng_L2-Big.dat


# L1-small configuration
echo "Configuring memory system for L1-small..."
L1_SIZE="L1_cache_size,4096\n"
L1_ASSOC="L1_assoc,1\n"
L2_SIZE="L2_cache_size,32768\n"
L2_ASSOC="L2_assoc,1\n"
echo -e "$SECT1$L1_SIZE$L1_ASSOC$SECT2$L2_SIZE$L2_ASSOC$SECT3" > config.csv

echo "Starting bzip2 simulation"
zcat ../traces-long/bzip2.gz | ./main config.csv > ./output/bzip2_L1-small.dat

echo "Starting h264ref simulation"
zcat ../traces-long/h264ref.gz | ./main config.csv > ./output/h264ref_L1-small.dat

echo "Starting libquantum simulation"
zcat ../traces-long/libquantum.gz | ./main config.csv > ./output/limquantum_L1-small.dat

echo "Starting omnetpp simulation"
zcat ../traces-long/omnetpp.gz | ./main config.csv > ./output/omnetpp_L1-small.dat

echo "Starting sjeng simulation"
zcat ../traces-long/sjeng.gz | ./main config.csv > ./output/sjeng_L1-small.dat


# L1-small-4way configuration
echo "Configuring memory system for L1-small-4way..."
L1_ASSOC="L1_assoc,4\n"
echo -e "$SECT1$L1_SIZE$L1_ASSOC$SECT2$L2_SIZE$L2_ASSOC$SECT3" > config.csv

echo "Starting bzip2 simulation"
zcat ../traces-long/bzip2.gz | ./main config.csv > ./output/bzip2_L1-small-4way.dat

echo "Starting h264ref simulation"
zcat ../traces-long/h264ref.gz | ./main config.csv > ./output/h264ref_L1-small-4way.dat

echo "Starting libquantum simulation"
zcat ../traces-long/libquantum.gz | ./main config.csv > ./output/limquantum_L1-small-4way.dat

echo "Starting omnetpp simulation"
zcat ../traces-long/omnetpp.gz | ./main config.csv > ./output/omnetpp_L1-small-4way.dat

echo "Starting sjeng simulation"
zcat ../traces-long/sjeng.gz | ./main config.csv > ./output/sjeng_L1-small-4way.dat


# All-FA configuration
echo "Configuring memory system for All-FA..."
L1_SIZE="L1_cache_size,8192\n"
L1_ASSOC="L1_assoc,256\n"
L2_SIZE="L2_cache_size,32768\n"
L2_ASSOC="L2_assoc,512\n"
echo -e "$SECT1$L1_SIZE$L1_ASSOC$SECT2$L2_SIZE$L2_ASSOC$SECT3" > config.csv

echo "Starting bzip2 simulation"
zcat ../traces-long/bzip2.gz | ./main config.csv > ./output/bzip2_All-FA.dat

echo "Starting h264ref simulation"
zcat ../traces-long/h264ref.gz | ./main config.csv > ./output/h264ref_All-FA.dat

echo "Starting libquantum simulation"
zcat ../traces-long/libquantum.gz | ./main config.csv > ./output/limquantum_All-FA.dat

echo "Starting omnetpp simulation"
zcat ../traces-long/omnetpp.gz | ./main config.csv > ./output/omnetpp_All-FA.dat

echo "Starting sjeng simulation"
zcat ../traces-long/sjeng.gz | ./main config.csv > ./output/sjeng_All-FA.dat


# All-FA-L2Big configuration
echo "Configuring memory system for All-FA-L2Big..."
L1_SIZE="L1_cache_size,8192\n"
L1_ASSOC="L1_assoc,256\n"
L2_SIZE="L2_cache_size,65536\n"
L2_ASSOC="L2_assoc,1024\n"
echo -e "$SECT1$L1_SIZE$L1_ASSOC$SECT2$L2_SIZE$L2_ASSOC$SECT3" > config.csv

echo "Starting bzip2 simulation"
zcat ../traces-long/bzip2.gz | ./main config.csv > ./output/bzip2_All-FA-L2Big.dat

echo "Starting h264ref simulation"
zcat ../traces-long/h264ref.gz | ./main config.csv > ./output/h264ref_All-FA-L2Big.dat

echo "Starting libquantum simulation"
zcat ../traces-long/libquantum.gz | ./main config.csv > ./output/limquantum_All-FA-L2Big.dat

echo "Starting omnetpp simulation"
zcat ../traces-long/omnetpp.gz | ./main config.csv > ./output/omnetpp_All-FA-L2Big.dat

echo "Starting sjeng simulation"
zcat ../traces-long/sjeng.gz | ./main config.csv > ./output/sjeng_All-FA-L2Big.dat
