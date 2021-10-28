#!/bin/bash
set -e
cd "$(dirname "$0")"

# Ignore comments and extra fields
ans=$(grep -v '^#' dataset/pequeno-gabarito.vcf | awk '{print $1"\t"$2"\t"$3"\t"$4"\t"$5}')
echo "Gabarito ($(echo "$ans" | wc -l) entradas):"
echo "$ans"

result=$(grep -f <(echo "$ans") amostra-lbb.vcf)
echo "Resultado ($(echo "$result" | wc -l) entradas):"
echo "$result"
