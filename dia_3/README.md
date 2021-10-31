# Solução do desafio Mendelics da LBB [(link)](https://github.com/mendelics/lbb-mendelics-2021/blob/main/Dia_3/README.md)

As tarefas foram implementadas em Bash. As mesmas dependências da [tarefa 2](../README.md) são usadas, com adição de [SnpEff](https://pcingola.github.io/SnpEff/se_inputoutput/).

## Tarefa

> Obtenha a razão Ti/Tv (transitions e transversions) das variantes encontrada no cromossomo 22

O arquivo `amostra-lbb-day2-capture-regions.vcf.gz` do dia 2 foi utilizado (o VCF de menor tamanho, contendo apenas regiões de captura do kit dadas na tarefa 2).

Segui passos dados na [documentação do snpEff](https://pcingola.github.io/SnpEff/se_inputoutput/) para realizar anotações. Primeiramente, o banco de dados do ncbi dbSNP2.0 foi usado para anotar variações comuns (https://www.ncbi.nlm.nih.gov/snp/docs/products/vcf/redesign/). Devido ao tamanho do arquivo e o tempo de competição, variações comuns (common) foram usadas ao invés de All. Por fim, o snpeff foi utilizado para anotar as variações, usando o genoma de referência hg38.

```
wget 'ftp://ftp.ncbi.nlm.nih.gov/snp/organisms/human_9606_b151_GRCh37p13/VCF/00-common_all.vcf.gz'
tabix amostra-lbb-day2-capture-regions.vcf.gz
tabix ./00-common_all.vcf.gz
bcftools annotate -a 00-common_all.vcf.gz -c ID -o amostra-lbb2-capture-regions-annotated.vcf  amostra-lbb-day2-capture-regions.vcf.gz

sudo mkdir -p /usr/share/java/snpEff/data/hg38  # pasta de download do genoma
sudo chown $USER:$USER /usr/share/java/snpEff/data/hg38
snpEff eff hg38 amostra-lbb2-capture-regions-annotated.vcf > amostra-lbb2-capture-regions-annotated-snpEff.vcf
```

O arquivo snpEff_summary.html indica:

* Transitions	636
* Transversions	235
* Ts/Tv ratio	2.7064

> Quantas variantes são encontradas na região de 16000000 a 20000000?

O arquivo ./amostra-lbb2-capture-regions-annotated-snpEff.vcf mostra variantes nessa região da linha 50 até a 123, ou seja, 74 variantes.

> Exiba o conteúdo da linha do VCF relativa a uma variante não-sinônima

Variantes não sinônimas resultam em uma alteração no aminoácido e na proteína, e estão marcadas como um dos subtipos de nonsynonymous_variant ([documentação](https://www.ebi.ac.uk/ols/ontologies/so/terms?iri=http%3A%2F%2Fpurl.obolibrary.org%2Fobo%2FSO_0001992&viewMode=All&siblings=false) - missense_variant, redundant_inserted_stop_gained, start_lost, stop_gained, stop_lost). Há muitas missense_variant e poucas das outras, por exemplo:

```
$ grep missense_variant amostra-lbb2-capture-regions-annotated-snpEff.vcf | head -n 5  # Limitar em 5 pois há muitas missense_variant
chr22   15528179        .       G       T       44.9447 .       DP=142;VDB=0.720191;SGB=-0.693097;RPBZ=0.977468;BQBZ=-0.818828;FS=0;MQ0F=0;AC=1;AN=2;DP4=80,17,27,3;MQ=42;ANN=T|missense_variant|MODERATE|OR11H1|OR11H1|transcript|NM_001005239.1|protein_coding|1/1|c.21G>T|p.Gln7His|21/981|21/981|7/326||       GT:PL   0/1:79,0,255
chr22   16591593        .       A       G       225.417 .       DP=155;VDB=0.00553653;SGB=-0.693147;FS=0;MQ0F=0;AC=2;AN=2;DP4=0,0,68,63;MQ=42;ANN=G|missense_variant|MODERATE|CCT8L2|CCT8L2|transcript|NM_014406.4|protein_coding|1/1|c.958T>C|p.Trp320Arg|1218/2053|958/1674|320/557||    GT:PL   1/1:255,255,0
chr22   16783675        .       G       T       99.6607 .       DP=42;VDB=0.0801279;SGB=-0.676189;RPBZ=-3.06318;BQBZ=-1.07661;FS=0;MQ0F=0;AC=1;AN=2;DP4=17,7,9,2;MQ=42;ANN=T|missense_variant|MODERATE|XKR3|XKR3|transcript|NM_001318251.1|protein_coding|4/4|c.1324C>A|p.His442Asn|1430/1689|1324/1380|442/459||,T|missense_variant|MODERATE|XKR3|XKR3|transcript|NM_175878.4|protein_coding|4/4|c.1324C>A|p.His442Asn|1427/1686|1324/1380|442/459|| GT:PL   0/1:133,0,255
chr22   16784234        .       A       C       222.396 .       DP=106;VDB=0.518333;SGB=-0.693147;RPBZ=1.26702;BQBZ=-1.97204;FS=0;MQ0F=0;AC=1;AN=2;DP4=15,30,11,41;MQ=42;ANN=C|missense_variant|MODERATE|XKR3|XKR3|transcript|NM_001318251.1|protein_coding|4/4|c.765T>G|p.Phe255Leu|871/1689|765/1380|255/459||,C|missense_variant|MODERATE|XKR3|XKR3|transcript|NM_175878.4|protein_coding|4/4|c.765T>G|p.Phe255Leu|868/1686|765/1380|255/459||     GT:PL   0/1:255,0,255
chr22   17108319        .       C       T       222.406 .       DP=87;VDB=0.726427;SGB=-0.693144;RPBZ=1.40984;BQBZ=-1.06575;FS=0;MQ0F=0;AC=1;AN=2;DP4=24,13,25,14;MQ=42;ANN=T|missense_variant|MODERATE|IL17RA|IL17RA|transcript|NM_014339.6|protein_coding|13/13|c.1100C>T|p.Ala367Val|1233/8608|1100/2601|367/866||,T|missense_variant|MODERATE|IL17RA|IL17RA|transcript|NM_001289905.1|protein_coding|12/12|c.998C>T|p.Ala333Val|1131/8506|998/2499|333/832||      GT:PL   0/1:255,0,255

$ grep redundant_inserted_stop_gained amostra-lbb2-capture-regions-annotated-snpEff.vcf  # Nenhuma

$ grep start_lost amostra-lbb2-capture-regions-annotated-snpEff.vcf
chr22   32479203        .       G       A       204.107 .       DP=98;VDB=0.69449;SGB=-0.69312;RPBZ=1.71234;BQBZ=-2.40952;FS=0;MQ0F=0;AC=1;AN=2;DP4=21,29,10,22;MQ=42;ANN=A|start_lost|HIGH|FBXO7|FBXO7|transcript|NM_001257990.1|protein_coding|2/9|c.3G>A|p.Met1?|385/1910|3/1227|1/408||,A|missense_variant|MODERATE|FBXO7|FBXO7|transcript|NM_012179.3|protein_coding|2/9|c.345G>A|p.Met115Ile|628/2153|345/1569|115/522||,A|missense_variant|MODERATE|FBXO7|FBXO7|transcript|NM_001033024.1|protein_coding|2/9|c.108G>A|p.Met36Ile|233/1758|108/1332|36/443||;LOF=(FBXO7|FBXO7|3|0.33)      GT:PL   0/1:237,0,255

$ grep stop_gained amostra-lbb2-capture-regions-annotated-snpEff.vcf
chr22   16988159        .       C       A       76.2337 .       DP=47;VDB=0.545078;SGB=-0.69168;RPBZ=-1.24531;BQBZ=-2.00175;FS=0;MQ0F=0;AC=1;AN=2;DP4=3,23,0,19;MQ=42;ANN=A|stop_gained|HIGH|GAB4|GAB4|transcript|NM_001037814.1|protein_coding|3/10|c.487G>T|p.Gly163*|595/2630|487/1725|163/574||;LOF=(GAB4|GAB4|1|1.00);NMD=(GAB4|GAB4|1|1.00)  GT:PL   0/1:109,0,210
chr22   41940168        .       G       A       225.417 .       DP=192;VDB=3.71066e-07;SGB=-0.693147;FS=0;MQ0F=0;AC=2;AN=2;DP4=0,0,60,98;MQ=42;ANN=A|stop_gained|HIGH|CENPM|CENPM|transcript|NM_001110215.2|protein_coding|1/2|c.7C>T|p.Arg3*|52/557|7/177|3/58||,A|downstream_gene_variant|MODIFIER|CENPM|CENPM|transcript|NM_001304371.1|protein_coding||c.*4968C>T|||||4753|,A|intron_variant|MODIFIER|CENPM|CENPM|transcript|NM_024053.4|protein_coding|5/5|c.403-972C>T||||||,A|intron_variant|MODIFIER|CENPM|CENPM|transcript|NM_001304370.1|protein_coding|4/4|c.301-972C>T||||||,A|intron_variant|MODIFIER|CENPM|CENPM|transcript|NM_001304372.1|protein_coding|4/4|c.*8-972C>T||||||,A|intron_variant|MODIFIER|CENPM|CENPM|transcript|NM_001304373.1|protein_coding|3/3|c.209-972C>T||||||,A|intron_variant|MODIFIER|CENPM|CENPM|transcript|NM_001002876.2|protein_coding|4/4|c.311-972C>T||||||   GT:PL   1/1:255,255,0


$ grep stop_lost amostra-lbb2-capture-regions-annotated-snpEff.vcf  # Nenhuma
```

> Variante no gnomAD v3.1.1 com MAF < 0.01

TODO
