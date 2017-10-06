# RCB File System

![Meerkats - by Stéphane Ente](meerkats.jpg "Meerkats - by Stéphane Enten")

O RCB é um sistema de arquivos baseado na implementação original do File Allocation Table (FAT), que utiliza uma lista ligada como método de alocação. Dessa forma, cada arquivo é composto por uma lista ligada de blocos do disco, evitando assim, desperdícios de memória com fragmentação interna pois, uma vez que o arquivo é composto por diversos setores que de alguma forma referenciam-se uns aos outros, é possivel que esses setores sejam espalhados no disco. O RCB utiliza 16 bits para endereçar os blocos.

O sistema de arquivos RCB enxerga a área de armazenamento como um conjunto de setores de tamanhos iguais. Os dois primeiros setores são reservados: o primeiro setor é o Boot Record, que armazena metadados fundamentais para a localização de qualquer dado no disco. O segundo setor armazena a tabela que mapeia os arquivos, ou seja, a lista ligada. O terceiro setor armazena tabelas de dados de entrada no diretório raiz. A partir do quarto setor, é destinado para o armazenamento dos arquivos e diretórios. Como utilizamos 16 bits para endereçamento dos setores, o nosso disco pode ter até 65536 setores independentemente da capacidade do disco. Visto que a tabela de alocação de arquivos aloca até 65536 linhas e que cada linha tem 2 bytes, é fato que a tabela de alocação de dados sempre terá 131072 bytes. Dessa forma, para descobrir quantos setores a tabela de alocação de dados ocupa, é necessário dividir 131072 pela quantidade de bytes por setor, que está especificado no Boot Record.

## Boot Record

Como dito anteriormente, o boot record é o primeiro bloco do sistema de arquivos. Nele, encontram-se metadados que servirão como referência para a localização de qualquer informação no disco. Um exemplo seria o endereço físico dos limites dos setores.
A tabela abaixo, indica o significado de cada metadado, o offset de início e o tamanho de cada um, ambos parâmetros informados a nível de bytes.

| Offset (em bytes) | Tamanho (em bytes) | Descrição |
| - | - | - |
| 0 | 2 | Assinatura do sistema de arquivos RCB |
| 2 | 2 | Quantidade de bytes por setor |
| 5 | 2 | Quantidade de setores reservados|
| 7 | 1 | Quantidade de entradas no diretório raiz |
| 8 | 2 | Capacidade em bytes da partição |

## Tabela de alocação de arquivos

É neste espaço que são armazenados os ponteiros seus respectivos status (ocupado, livre) de todos os blocos do disco. Cada linha deste setor contem um valor de 2 bytes. O índice da linha indica o endereço do espeço do dado em questão, e o conteúdo da linha representa o próximo índice do dado em questão. Este valor pode ser (em hexadecimal): 
Qualquer outro valor = Representa o próximo índice da sequência do dado.
FFFF = A sequência do dado chegou ao fim
0000 = O espaço está vazio

## Tabela de Dados de Diretórios

Todos os diretórios da partição terá a seguinte tabela para cada entrada.

| Offset (em bytes) | Tamanho (em bytes) | Descrição |
| - | - | - |
| 0 | 11 | Nome do arquivo, em que os 8 primeiros caracteres são o nome e os três ultimos a extenção |
| 11 | 1 | Atributo do arquivo, podendo ser: diretório (0x01) ou arquivo (0x10) |
| 12 | 1 | Identificação do primeiro cluster do arquivo |
| 13 | 4 | Tamanho do arquivo em bytes |
