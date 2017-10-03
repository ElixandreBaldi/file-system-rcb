# RCB File System

![Meerkats - by Stéphane Ente](meerkats.jpg "Meerkats - by Stéphane Enten")

O RCB é um sistema de arquivos baseado na implementação original do File Allocation Table (FAT), que utiliza uma lista ligada como método de alocação. Dessa forma, cada arquivo é composto por uma lista ligada de blocos do disco, evitando assim, desperdícios de memória com fragmentação interna pois, uma vez que o arquivo é composto por diversos setores que de alguma forma referenciam-se uns aos outros, é possivel que esses setores sejam espalhados no disco. O RCB utiliza 16 bits para endereçar os blocos.

O sistema de arquivos RCB enxerga a área de armazenamento como um conjunto de setores de tamanhos iguais. Os três primeiros setores são reservados: o primeiro setor é o Boot Record, que armazena metadados fundamentais para a localização de qualquer dado no disco. O segundo setor armazena a tabela que mapeia os arquivos, ou seja, a lista ligada. O terceiro setor armazena uma tabela dos dados no diretório raiz. A partir do quarto setor, é destinado para o armazenamento dos arquivos e diretórios. Como utilizamos 16 bits para endereçamento dos setores, o nosso disco pode ter até 65536 setores independentemente da capacidade do disco. Visto que a tabela de alocação de arquivos aloca até 65536 linhas e que cada linha tem 2 bytes, é ideal que cada setor tenha no mínimo 131072 bytes, neste caso, o disco teria no mínimo 8589934592 bytes de capacidade ou 8 GB.

## Boot Record

Como dito anteriormente, o boot record é o primeiro bloco do sistema de arquivos. Nele, encontram-se metadados que servirão como referência para a localização de qualquer informação no disco. Um exemplo seria o endereço físico dos limites dos blocos.
A tabela abaixo, indica o significado de cada metadado, o offset de início e o tamanho de cada um, ambos parâmetros informados a nível de bytes.

| Offset (em bytes) | Tamanho (em bytes) | Descrição |
| - | - | - |
| 0 | 2 | Assinatura do sistema de arquivos RCB |
| 2 | 2 | Quantidade de bytes por blocos |
| 5 | 2 | Quantidade de blocos reservados|
| 7 | 1 | Quantidade de entradas no diretório raiz |
| 8 | 2 | Capacidade em bytes da partição |

## Tabela de alocação de arquivos

É neste espaço que são armazenados os ponteiros seus respectivos status (ocupado, livre) de todos os blocos do disco.

## Tabela de Dados

| Offset (em bytes) | Tamanho (em bytes) | Descrição |
| - | - | - |
| 0 | 11 | Nome do arquivo, em que os 8 primeiros caracteres são o nome e os três ultimos a extenção |
| 11 | 1 | Atributo do arquivo, podendo ser: diretório (0x01) ou arquivo (0x10) |
| 12 | 1 | Identificação do primeiro cluster do arquivo |
| 13 | 4 | Tamanho do arquivo em bytes |
