#!/bin/bash
set -e
cd "$(dirname "$0")"

echo 'Cleaning repository'
git clean -dfx

if [ ! -e venv ] ; then
    pypy3 -m venv venv
fi
. venv/bin/activate

echo 'Indexing reference genome...'
bowtie2-build dataset/grch38.chr22.fasta.gz index/grch38.chr22

echo 'Recompressing with bgzip...'
zcat dataset/grch38.chr22.fasta.gz | bgzip > dataset/grch38.chr22.fasta.bgz.gz

echo 'Mapping to reference genome...'
bowtie2 -x index/grch38.chr22 -1 dataset/amostra-lbb_R1.fq.gz -2 dataset/amostra-lbb_R2.fq.gz > mapped.sam

echo 'Converting to BAM...'
samtools sort mapped.sam -o mapped.bam
bcftools mpileup -Ou -f dataset/grch38.chr22.fasta.bgz.gz mapped.bam | bcftools call -mv -Oz -o amostra-lbb.vcf.gz
