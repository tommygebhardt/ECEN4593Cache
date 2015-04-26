#!/bin/bash
# This script runs cache simulations in which the main memory bandwidth is
# increased by powers of 2 (from 16 to 32 to 64).  The idea is to compare
# performance results to cost.

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
L2_bus_width,16\n"
MM_CHUNK="MM_chunksize,8"

# Default configuration
echo "Configuring memory system for Default..."
MM_CHUNK="MM_chunksize,16"
echo -e "$SECT1$L1_SIZE$L1_ASSOC$SECT2$L2_SIZE$L2_ASSOC$SECT3$MM_CHUNK" > config.csv
zcat ../traces-long/sjeng.gz | ./main config.csv > ./output/sjeng_Default16.dat
MM_CHUNK="MM_chunksize,32"
echo -e "$SECT1$L1_SIZE$L1_ASSOC$SECT2$L2_SIZE$L2_ASSOC$SECT3$MM_CHUNK" > config.csv
zcat ../traces-long/sjeng.gz | ./main config.csv > ./output/sjeng_Default32.dat
MM_CHUNK="MM_chunksize,64"
echo -e "$SECT1$L1_SIZE$L1_ASSOC$SECT2$L2_SIZE$L2_ASSOC$SECT3$MM_CHUNK" > config.csv
zcat ../traces-long/sjeng.gz | ./main config.csv > ./output/sjeng_Default64.dat

# L1-2way configuration
echo "Configuring memory system for L1-2way..."
L1_ASSOC="L1_assoc,2\n"
MM_CHUNK="MM_chunksize,16"
echo -e "$SECT1$L1_SIZE$L1_ASSOC$SECT2$L2_SIZE$L2_ASSOC$SECT3$MM_CHUNK" > config.csv
zcat ../traces-long/sjeng.gz | ./main config.csv > ./output/sjeng_L1-2way16.dat
MM_CHUNK="MM_chunksize,32"
echo -e "$SECT1$L1_SIZE$L1_ASSOC$SECT2$L2_SIZE$L2_ASSOC$SECT3$MM_CHUNK" > config.csv
zcat ../traces-long/sjeng.gz | ./main config.csv > ./output/sjeng_L1-2way32.dat
MM_CHUNK="MM_chunksize,64"
echo -e "$SECT1$L1_SIZE$L1_ASSOC$SECT2$L2_SIZE$L2_ASSOC$SECT3$MM_CHUNK" > config.csv
zcat ../traces-long/sjeng.gz | ./main config.csv > ./output/sjeng_L1-2way64.dat

# All-2way configuration
echo "Configuring memory system for All-2way..."
L2_ASSOC="L2_assoc,2\n"
MM_CHUNK="MM_chunksize,16"
echo -e "$SECT1$L1_SIZE$L1_ASSOC$SECT2$L2_SIZE$L2_ASSOC$SECT3$MM_CHUNK" > config.csv
zcat ../traces-long/sjeng.gz | ./main config.csv > ./output/sjeng_All-2way16.dat
MM_CHUNK="MM_chunksize,32"
echo -e "$SECT1$L1_SIZE$L1_ASSOC$SECT2$L2_SIZE$L2_ASSOC$SECT3$MM_CHUNK" > config.csv
zcat ../traces-long/sjeng.gz | ./main config.csv > ./output/sjeng_All-2way32.dat
MM_CHUNK="MM_chunksize,64"
echo -e "$SECT1$L1_SIZE$L1_ASSOC$SECT2$L2_SIZE$L2_ASSOC$SECT3$MM_CHUNK" > config.csv
zcat ../traces-long/sjeng.gz | ./main config.csv > ./output/sjeng_All-2way64.dat


# L2-4way configuration
echo "Configuring memory system for L2-4way..."
L2_ASSOC="L2_assoc,4\n"
MM_CHUNK="MM_chunksize,16"
echo -e "$SECT1$L1_SIZE$L1_ASSOC$SECT2$L2_SIZE$L2_ASSOC$SECT3$MM_CHUNK" > config.csv
zcat ../traces-long/sjeng.gz | ./main config.csv > ./output/sjeng_L2-4way16.dat
MM_CHUNK="MM_chunksize,32"
echo -e "$SECT1$L1_SIZE$L1_ASSOC$SECT2$L2_SIZE$L2_ASSOC$SECT3$MM_CHUNK" > config.csv
zcat ../traces-long/sjeng.gz | ./main config.csv > ./output/sjeng_L2-4way32.dat
MM_CHUNK="MM_chunksize,64"
echo -e "$SECT1$L1_SIZE$L1_ASSOC$SECT2$L2_SIZE$L2_ASSOC$SECT3$MM_CHUNK" > config.csv
zcat ../traces-long/sjeng.gz | ./main config.csv > ./output/sjeng_L2-4way64.dat


# All-4way configuration
echo "Configuring memory system for All-4way..."
L1_ASSOC="L1_assoc,4\n"
MM_CHUNK="MM_chunksize,16"
echo -e "$SECT1$L1_SIZE$L1_ASSOC$SECT2$L2_SIZE$L2_ASSOC$SECT3$MM_CHUNK" > config.csv
zcat ../traces-long/sjeng.gz | ./main config.csv > ./output/sjeng_All-4way16.dat
MM_CHUNK="MM_chunksize,32"
echo -e "$SECT1$L1_SIZE$L1_ASSOC$SECT2$L2_SIZE$L2_ASSOC$SECT3$MM_CHUNK" > config.csv
zcat ../traces-long/sjeng.gz | ./main config.csv > ./output/sjeng_All-4way32.dat
MM_CHUNK="MM_chunksize,64"
echo -e "$SECT1$L1_SIZE$L1_ASSOC$SECT2$L2_SIZE$L2_ASSOC$SECT3$MM_CHUNK" > config.csv
zcat ../traces-long/sjeng.gz | ./main config.csv > ./output/sjeng_All-4way64.dat


# L1-8way configuration
echo "Configuring memory system for L1-8way..."
L1_ASSOC="L1_assoc,8\n"
L2_ASSOC="L2_assoc,1\n"
MM_CHUNK="MM_chunksize,16"
echo -e "$SECT1$L1_SIZE$L1_ASSOC$SECT2$L2_SIZE$L2_ASSOC$SECT3$MM_CHUNK" > config.csv
zcat ../traces-long/sjeng.gz | ./main config.csv > ./output/sjeng_L1-8way16.dat
MM_CHUNK="MM_chunksize,32"
echo -e "$SECT1$L1_SIZE$L1_ASSOC$SECT2$L2_SIZE$L2_ASSOC$SECT3$MM_CHUNK" > config.csv
zcat ../traces-long/sjeng.gz | ./main config.csv > ./output/sjeng_L1-8way32.dat
MM_CHUNK="MM_chunksize,64"
echo -e "$SECT1$L1_SIZE$L1_ASSOC$SECT2$L2_SIZE$L2_ASSOC$SECT3$MM_CHUNK" > config.csv
zcat ../traces-long/sjeng.gz | ./main config.csv > ./output/sjeng_L1-8way64.dat


# L2-Big configuration
echo "Configuring memory system for L2-Big..."
L1_ASSOC="L1_assoc,2\n"
L2_SIZE="L2_cache_size,65536\n"
MM_CHUNK="MM_chunksize,16"
echo -e "$SECT1$L1_SIZE$L1_ASSOC$SECT2$L2_SIZE$L2_ASSOC$SECT3$MM_CHUNK" > config.csv
zcat ../traces-long/sjeng.gz | ./main config.csv > ./output/sjeng_L2-Big16.dat
MM_CHUNK="MM_chunksize,32"
echo -e "$SECT1$L1_SIZE$L1_ASSOC$SECT2$L2_SIZE$L2_ASSOC$SECT3$MM_CHUNK" > config.csv
zcat ../traces-long/sjeng.gz | ./main config.csv > ./output/sjeng_L2-Big32.dat
MM_CHUNK="MM_chunksize,64"
echo -e "$SECT1$L1_SIZE$L1_ASSOC$SECT2$L2_SIZE$L2_ASSOC$SECT3$MM_CHUNK" > config.csv
zcat ../traces-long/sjeng.gz | ./main config.csv > ./output/sjeng_L2-Big64.dat


# L1-small configuration
echo "Configuring memory system for L1-small..."
L1_SIZE="L1_cache_size,4096\n"
L1_ASSOC="L1_assoc,1\n"
L2_SIZE="L2_cache_size,32768\n"
L2_ASSOC="L2_assoc,1\n"
MM_CHUNK="MM_chunksize,16"
echo -e "$SECT1$L1_SIZE$L1_ASSOC$SECT2$L2_SIZE$L2_ASSOC$SECT3$MM_CHUNK" > config.csv
zcat ../traces-long/sjeng.gz | ./main config.csv > ./output/sjeng_L1-small16.dat
MM_CHUNK="MM_chunksize,32"
echo -e "$SECT1$L1_SIZE$L1_ASSOC$SECT2$L2_SIZE$L2_ASSOC$SECT3$MM_CHUNK" > config.csv
zcat ../traces-long/sjeng.gz | ./main config.csv > ./output/sjeng_L1-small32.dat
MM_CHUNK="MM_chunksize,64"
echo -e "$SECT1$L1_SIZE$L1_ASSOC$SECT2$L2_SIZE$L2_ASSOC$SECT3$MM_CHUNK" > config.csv
zcat ../traces-long/sjeng.gz | ./main config.csv > ./output/sjeng_L1-small64.dat


# L1-small-4way configuration
echo "Configuring memory system for L1-small-4way..."
L1_ASSOC="L1_assoc,4\n"
MM_CHUNK="MM_chunksize,16"
echo -e "$SECT1$L1_SIZE$L1_ASSOC$SECT2$L2_SIZE$L2_ASSOC$SECT3$MM_CHUNK" > config.csv
zcat ../traces-long/sjeng.gz | ./main config.csv > ./output/sjeng_L1-small-4way16.dat
MM_CHUNK="MM_chunksize,32"
echo -e "$SECT1$L1_SIZE$L1_ASSOC$SECT2$L2_SIZE$L2_ASSOC$SECT3$MM_CHUNK" > config.csv
zcat ../traces-long/sjeng.gz | ./main config.csv > ./output/sjeng_L1-small-4way32.dat
MM_CHUNK="MM_chunksize,64"
echo -e "$SECT1$L1_SIZE$L1_ASSOC$SECT2$L2_SIZE$L2_ASSOC$SECT3$MM_CHUNK" > config.csv
zcat ../traces-long/sjeng.gz | ./main config.csv > ./output/sjeng_L1-small-4way64.dat

# All-FA configuration
echo "Configuring memory system for All-FA..."
L1_SIZE="L1_cache_size,8192\n"
L1_ASSOC="L1_assoc,256\n"
L2_SIZE="L2_cache_size,32768\n"
L2_ASSOC="L2_assoc,512\n"
MM_CHUNK="MM_chunksize,16"
echo -e "$SECT1$L1_SIZE$L1_ASSOC$SECT2$L2_SIZE$L2_ASSOC$SECT3$MM_CHUNK" > config.csv
zcat ../traces-long/sjeng.gz | ./main config.csv > ./output/sjeng_All-FA16.dat
MM_CHUNK="MM_chunksize,32"
echo -e "$SECT1$L1_SIZE$L1_ASSOC$SECT2$L2_SIZE$L2_ASSOC$SECT3$MM_CHUNK" > config.csv
zcat ../traces-long/sjeng.gz | ./main config.csv > ./output/sjeng_All-FA32.dat
MM_CHUNK="MM_chunksize,64"
echo -e "$SECT1$L1_SIZE$L1_ASSOC$SECT2$L2_SIZE$L2_ASSOC$SECT3$MM_CHUNK" > config.csv
zcat ../traces-long/sjeng.gz | ./main config.csv > ./output/sjeng_All-FA64.dat


# All-FA-L2Big configuration
echo "Configuring memory system for All-FA-L2Big..."
L1_SIZE="L1_cache_size,8192\n"
L1_ASSOC="L1_assoc,256\n"
L2_SIZE="L2_cache_size,65536\n"
L2_ASSOC="L2_assoc,1024\n"
MM_CHUNK="MM_chunksize,16"
echo -e "$SECT1$L1_SIZE$L1_ASSOC$SECT2$L2_SIZE$L2_ASSOC$SECT3$MM_CHUNK" > config.csv
zcat ../traces-long/sjeng.gz | ./main config.csv > ./output/sjeng_All-FA-L2Big16.dat
MM_CHUNK="MM_chunksize,32"
echo -e "$SECT1$L1_SIZE$L1_ASSOC$SECT2$L2_SIZE$L2_ASSOC$SECT3$MM_CHUNK" > config.csv
zcat ../traces-long/sjeng.gz | ./main config.csv > ./output/sjeng_All-FA-L2Big32.dat
MM_CHUNK="MM_chunksize,64"
echo -e "$SECT1$L1_SIZE$L1_ASSOC$SECT2$L2_SIZE$L2_ASSOC$SECT3$MM_CHUNK" > config.csv
zcat ../traces-long/sjeng.gz | ./main config.csv > ./output/sjeng_All-FA-L2Big64.dat
