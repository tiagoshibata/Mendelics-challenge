# Solução do desafio Mendelics da LBB [(link)](https://github.com/mendelics/lbb-mendelics-2021/blob/main/Dia_2/README.md)

As tarefas foram implementadas em Bash e C++. As mesmas dependências da [tarefa 1](../README.md) são usadas, com adição de [sambamba](https://lomereiter.github.io/sambamba/index.html) para filtragem de arquivos BAM, e clang para compilar C++.

## Tarefa

> Quais variantes deverão ser desconsideradas no seu VCF? - Qualquer métrica do software de escolha poderá ser utilizada. Discorra sobre a métrica utilizada.

O pipeline pode ser visto em [clean-run.sh](clean-run.sh).

Eu mantive os mesmos *scores* padrões do bowtie2, que já são robustos o suficiente. O parâmetro `--very-sensitive` foi adicionado ao bowtie2 para realizar mais buscas e possivelmente encontrar mais regiões similares, o que nos dá maior confiança no valor de qualidade do mapeamento (MAPQ) gerado. O parâmetro `--no-unal` também foi adicionado para não colocar na saída segmentos que não tenham alinhado, o que gera um arquivo um pouco menor e sem entradas que não utilizaríamos.

Algumas etapas de filtragem foram adicionadas. Primeiramente, segmentos onde o bowtie2 encontrou outro alinhamento com bom *score* no genoma de referência foram descartados. Isso foi feito removendo todos os segmentos que tivessem a tag XS (essa tag é colocada pelo bowtie2 apenas se o segmento possui outro alinhamento bom, e seu valor é o *score* do segundo melhor alinhamento). Segmentos com MAPQ < 20 também foram removidos.

Após essas etapas de filtragem, o BAM reduziu de 48M (1714279 segmentos) para 40M (1444854 segmentos). Os BAMs estão inclusos na mesma pasta desse README.md.

O VCF filtrado possui 3746 variantes, em contraste com 4024 antes da filtragem. Se delimitarmos para as regiões de captura do kit, o VCF possui 711 entradas. Os dois VCFs estão disponíveis em [amostra-lbb-day2.vcf.gz](amostra-lbb-day2.vcf.gz) (VCF filtrado) e [amostra-lbb-day2-capture-regions.vcf.gz](amostra-lbb-day2-capture-regions.vcf.gz) (VCF filtrado e delimitado para as regiões de captura dadas no enunciado).

> Discorra sobre as regiões com baixa cobertura e quais foram seus critérios. Figuras são bem-vindas.

O programa coverage.cpp calcula a cobertura para cada posição no genoma e gera um BED com o resultado. Além disso, para cada região de captura do kit, ele calcula a cobertura máxima e mínima que há naquela região, e gera uma imagem para visualização da cobertura na região.

Observei que 190 das regiões de captura do kit não estão cobertas, e mais algumas estão parcialmente cobertas (possuem posições com cobertura 0), o que indica que a minha filtragem foi excessiva, ou meus parâmetros de mapeamento devem ser ajustados, ou a amostra não cobriu essas regiões.

A cobertura em cada região está na pasta [images](../images). Cada pixel na imagem é escuro se não tiver cobertura, e se torna mais colorido quanto maior a cobertura. Por exemplo, na região `TANGO2_4`, que possui boa cobertura:

![TANGO24](./TANGO2_4.png)

E na `AC004471.1_1`, que não está muito coberta no início:

![AC004471.11](./AC004471.1_1.png)

Informações das regiões de captura (posição com menor e menor cobertura para cada região) estão em [coverage.txt](coverage.txt).
