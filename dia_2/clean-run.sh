#!/bin/bash
set -e
cd "$(dirname "$0")"/..

echo 'Cleaning repository'
git clean -dfx

echo 'Indexing reference genome...'
bowtie2-build dataset/grch38.chr22.fasta.gz index/grch38.chr22

echo 'Recompressing with bgzip...'
zcat dataset/grch38.chr22.fasta.gz | bgzip > dataset/grch38.chr22.fasta.bgz.gz

echo 'Mapping to reference genome...'
bowtie2 -D200 --very-sensitive -D200 -p 24 -x index/grch38.chr22 -1 dataset/amostra-lbb_R1.fq.gz -2 dataset/amostra-lbb_R2.fq.gz --no-unal > mapped_day2.sam

# echo 'Filtering mappings with low quality...'
# pypy3 filter.py < mapped_day2.sam > filtered.sam

echo 'Converting to BAM...'
samtools sort mapped_day2.sam -o mapped_day2.bam
sambamba view -h -t 24 -f bam -F '[XS] == null and mapping_quality >= 20' mapped_day2.bam > filtered_day2.bam

echo 'Creating VCF...'
bcftools mpileup -Ou -f dataset/grch38.chr22.fasta.bgz.gz filtered_day2.bam | bcftools call -mv -Oz -o amostra-lbb-day2.vcf.gz

echo 'Limiting to capture regions...'
tabix -p vcf amostra-lbb-day2.vcf.gz
tabix -R dataset/coverage.bed.gz amostra-lbb-day2.vcf.gz > amostra-lbb-day2-capture-regions.vcf
bgzip amostra-lbb-day2-capture-regions.vcf
# bcftools mpileup -C10 -Ou -f dataset/grch38.chr22.fasta.bgz.gz mapped.bam | bcftools call -mv -Oz -o amostra-lbb.vcf.gz

echo 'Analysing regions with low coverage...'
clang++ -std=c++17 -Wall -Wextra -O3 dia_2/coverage.cpp -o coverage
./coverage <(samtools view filtered_day2.bam) <(zcat dataset/coverage.bed.gz)
