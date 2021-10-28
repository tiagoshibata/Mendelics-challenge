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

# for f in amostra-lbb_R1 amostra-lbb_R2 ; do
#     echo 'Mapping to reference genome...'
#     # minimap2 -cx sr --cs -t24 --sr -o $f.paf dataset/grch38.chr22.fasta.gz dataset/$f.fq.gz
#     # TODO -l 1000 -L 5000
#     # sort -k6,6 -k8,8n $f.paf | paftools.js call -l 10 -L 50 -f dataset/grch38.chr22.fasta.gz - > $f.vcf
#     bowtie2 -x index/grch38.chr22 -U dataset/$f.fq.gz > $f.sam
#     echo 'Converting to BAM...'
#     # samtools view -S -b $f.sam > ${f}_unsorted.bam
#     samtools sort $f.sam -o $f.bam
#     bcftools mpileup -Ou -f dataset/grch38.chr22.fasta.bgz.gz $f.bam | bcftools call -mv -o $f.vcf
# done

echo 'Mapping to reference genome...'
# minimap2 -cx sr --cs -t24 --sr -o $f.paf dataset/grch38.chr22.fasta.gz dataset/$f.fq.gz
# TODO -l 1000 -L 5000
# sort -k6,6 -k8,8n $f.paf | paftools.js call -l 10 -L 50 -f dataset/grch38.chr22.fasta.gz - > $f.vcf
bowtie2 -x index/grch38.chr22 -1 dataset/amostra-lbb_R1.fq.gz -2 dataset/amostra-lbb_R2.fq.gz > mapped.sam
echo 'Converting to BAM...'
# samtools view -S -b $f.sam > ${f}_unsorted.bam
samtools sort mapped.sam -o mapped.bam
bcftools mpileup -Ou -f dataset/grch38.chr22.fasta.bgz.gz mapped.bam | bcftools call -mv -o amostra-lbb.vcf
