#!/bin/bash
set -e
cd "$(dirname "$0")"

echo 'Cleaning repository'
git clean -dfx

echo 'Indexing reference genome...'
bowtie2-build dataset/grch38.chr22.fasta.gz index/grch38.chr22

echo 'Recompressing with bgzip...'
zcat dataset/grch38.chr22.fasta.gz | bgzip > dataset/grch38.chr22.fasta.bgz.gz

echo 'Mapping to reference genome...'
bowtie2 -p 24 -x index/grch38.chr22 -1 dataset/amostra-lbb_R1.fq.gz -2 dataset/amostra-lbb_R2.fq.gz > mapped.sam
# bowtie2 -D200 --very-sensitive -D200 --score-min 'L,-0.9,-0.9' -p 24 -x index/grch38.chr22 -1 dataset/amostra-lbb_R1.fq.gz -2 dataset/amostra-lbb_R2.fq.gz > mapped.sam

echo 'Converting to BAM...'
samtools sort mapped.sam -o mapped.bam
bcftools mpileup -Ou -f dataset/grch38.chr22.fasta.bgz.gz mapped.bam | bcftools call -mv -Oz -o amostra-lbb.vcf.gz
# bcftools mpileup -C10 -Ou -f dataset/grch38.chr22.fasta.bgz.gz mapped.bam | bcftools call -mv -Oz -o amostra-lbb.vcf.gz
