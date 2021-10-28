# Solução do desafio Mendelics da LBB [(link)](https://github.com/mendelics/lbb-mendelics-2021/blob/main/Dia_1/README.md)

## Tarefa 1

A tarefa foi implementada em bash.

Prerequisitos:

* Bash
* bcftools, bowtie2 e samtools para análise das sequências
    * Links para instalação pelo Arch User Repository: [bcftools](https://aur.archlinux.org/packages/bcftools/), [bowtie2-bin](https://aur.archlinux.org/packages/bowtie2-bin/), [samtools](https://aur.archlinux.org/packages/samtools/)
* Programas como awk, bgzip, gzip e zcat, que costumam estar disponíveis em qualquer distribuição Linux

O pipeline para geração do VCF está em [clean-run.sh](clean-run.sh).

* bowtie2 é utilizado para alinhar as sequências ao genoma de referência
    * Um índice do genoma de referência é gerado com `bowtie2-build`. O índice acelera operações de busca na etapa de mapeamento
    * `bowtie2` gera um arquivo .sam com o mapeamento das sequências no genoma de referência
* samtools é utilizado para converter o .sam para um .bam (representação binária e mais compacta do .sam)
* bcftools é utilizado para gerar um VCF a partir do .bam

Os resultados foram validados com o script [validate.sh](validate.sh), que busca se as entradas do gabarito estão presentes no VCF gerado. Resultados:

```
Gabarito (10 entradas):
chr22   19039100        .       A       G
chr22   19764306        .       C       T
chr22   21975710        .       T       C
chr22   23069838        .       A       G
chr22   26027014        .       A       G
chr22   29441577        .       TTCC    T
chr22   33006594        .       C       T
chr22   35264882        .       G       T
chr22   37746461        .       A       G
chr22   43180898        .       G       A
Resultado (9 entradas):
chr22   19039100        .       A       G       222.318 .       DP=243;VDB=0.0689368;SGB=-0.693147;RPBZ=-1.80982;MQBZ=-1.14316;MQSBZ=1.13177;BQBZ=-0.628896;FS=0;MQ0F=0;AC=1;AN=2;DP4=55,60,34,54;MQ=41    GT:PL   0/1:255,0,255
chr22   19764306        .       C       T       222.327 .       DP=96;VDB=0.666932;SGB=-0.693147;RPBZ=-1.15035;BQBZ=0.517154;FS=0;MQ0F=0;AC=1;AN=2;DP4=9,26,12,33;MQ=42 GT:PL   0/1:255,0,255
chr22   21975710        .       T       C       213.108 .       DP=68;VDB=0.49071;SGB=-0.69168;RPBZ=1.02261;BQBZ=0.671389;FS=0;MQ0F=0;AC=1;AN=2;DP4=8,26,9,10;MQ=42     GT:PL   0/1:246,0,255
chr22   23069838        .       A       G       193.403 .       DP=134;VDB=0.974027;SGB=-0.693147;RPBZ=1.34553;BQBZ=-0.763303;FS=0;MQ0F=0;AC=1;AN=2;DP4=9,51,3,61;MQ=42 GT:PL   0/1:226,0,255
chr22   26027014        .       A       G       225.417 .       DP=192;VDB=0.23656;SGB=-0.693147;FS=0;MQ0F=0;AC=2;AN=2;DP4=0,0,109,46;MQ=42     GT:PL   1/1:255,255,0
chr22   33006594        .       C       T       222.332 .       DP=90;VDB=0.980837;SGB=-0.693097;RPBZ=1.95785;BQBZ=1.12233;FS=0;MQ0F=0;AC=1;AN=2;DP4=23,18,16,14;MQ=42  GT:PL   0/1:255,0,255
chr22   35264882        .       G       T       225.417 .       DP=53;VDB=0.527811;SGB=-0.693147;FS=0;MQ0F=0;AC=2;AN=2;DP4=0,0,29,19;MQ=42      GT:PL   1/1:255,144,0
chr22   37746461        .       A       G       225.417 .       DP=55;VDB=0.0999968;SGB=-0.693146;FS=0;MQ0F=0;AC=2;AN=2;DP4=0,0,21,22;MQ=42     GT:PL   1/1:255,129,0
chr22   43180898        .       G       A       202.179 .       DP=71;VDB=0.443898;SGB=-0.692562;RPBZ=0.529221;BQBZ=-0.546718;FS=0;MQ0F=0;AC=1;AN=2;DP4=5,29,5,17;MQ=42 GT:PL   0/1:235,0,255
```

9/10 entradas do gabarito estão presentes. A entrada `chr22   29441577        .       TTCC    T` não foi encontrada. Testei o mapeamento com outros parâmetros e com as ferramentas minimap2 e bbmap, ou ajustar o score do bcftools, e não consegui um bom match para essa entrada.

Vídeo com execução do pipeline e script de validação:

https://asciinema.org/a/445361
