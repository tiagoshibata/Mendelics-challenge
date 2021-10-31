# Solução do desafio Mendelics da LBB [(link)](https://github.com/mendelics/lbb-mendelics-2021/blob/main/Dia_3/README.md)

As tarefas foram implementadas em Bash. As mesmas dependências da [tarefa 2](../README.md) são usadas, com adição de [SnpEff](https://pcingola.github.io/SnpEff/se_inputoutput/).

## Tarefa

> Obtenha a razão Ti/Tv (transitions e transversions) das variantes encontrada no cromossomo 22

O arquivo `amostra-lbb-day2-capture-regions.vcf.gz` do dia 2 foi utilizado.

Segui passos dados na [documentação do snpEff](https://pcingola.github.io/SnpEff/se_inputoutput/) para realizar anotações. Primeiramente, o banco de dados do ncbi dbSNP2.0 foi usado para anotar variações comuns (https://www.ncbi.nlm.nih.gov/snp/docs/products/vcf/redesign/). Devido ao tamanho do arquivo e o tempo de competição, variações comuns (common) foram usadas ao invés de All.


