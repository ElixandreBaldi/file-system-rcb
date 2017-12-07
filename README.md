# RCB File System

![Meerkats - by Stéphane Ente](meerkats.jpg "Meerkats - by Stéphane Enten")

O RCB é um sistema de arquivos baseado na implementação original do File Allocation Table (FAT), que utiliza uma lista ligada como método de alocação. Dessa forma, cada arquivo é composto por uma lista ligada de blocos do disco, evitando assim, desperdícios de memória com fragmentação interna pois, uma vez que o arquivo é composto por diversos setores que de alguma forma referenciam-se uns aos outros, é possivel que esses setores sejam espalhados no disco. O RCB utiliza 16 bits para endereçar os blocos.

O sistema de arquivos RCB enxerga a área de armazenamento como um conjunto de setores de tamanhos iguais. O primeiro setor e os próximos n setores que a tabela de alocação de arquivos está comportada são reservados. O primeiro setor é o Boot Record, que armazena metadados fundamentais para a localização de qualquer dado no disco. Os próximos n setores armazena a tabela de alocação de arquivo e a partir do setor (tamanho da tabela RCB + 1) é destinado exclusivamente para o diretório raiz, as posiçes seguintes são utilizadas para arquivos ou sub-diretórios. O valor n está definido no Boot Record como quantidade de setores da tabela de alocação de arquivos.

## Boot Record

Como dito anteriormente, o boot record é o primeiro bloco do sistema de arquivos. Nele, encontram-se metadados que servirão como referência para a localização de qualquer informação no disco. Um exemplo seria o endereço físico dos limites dos setores.
A tabela abaixo, indica o significado de cada metadado, o offset de início e o tamanho de cada um, ambos parâmetros informados a nível de bytes.

| Offset (em bytes) | Tamanho (em bytes) | Descrição |
| - | - | - |
| 0 | 2 | Assinatura do sistema de arquivos RCB |
| 2 | 2 | Quantidade de bytes por setor |
| 4 | 2 | Quantidade de setores reservados|
| 6 | 2 | Quantidade de entradas por diretório |
| 8 | 4 | Capacidade em bytes da partição |
| 12 | 2 | Quantidade de setores da tabela de alocação de arquivos |
| 14 | 2 | Quantidade de setores que existem fisicamente no disco |

O mínimo de setores existentes no sistema de arquivos RCB é de quatro (o primeiro setor reservado ao boot record, o segundo destinado a tabela RCB, o terceiro para o diretório raiz, e o quarto para a área de dados). A quantidade máxima de setores são exatamente a quantidade de linhas da tabela RCB, que seria 2^(16)+tamanho do boot record + tamanho da tabela RBC. A descrição do tamanho da tabela RCB é descrita no escopo do texto. Outra restrição, é que um setor nunca terá menos do que 16 bytes, visto que o boot record necessita estar em um setor, e o mesmo tem tamanho de 16 bytes.

## Tabela de alocação de arquivos RCB

É neste espaço que são armazenados os ponteiros seus respectivos status (ocupado, livre) de todos os blocos do disco. Cada linha deste setor contem um valor de 2 bytes. O índice da linha indica o endereço do espeço do dado em questão, e o conteúdo da linha representa o próximo índice do dado em questão. A tabela abaixo representa os valores possíveis por linha. 

| Valor em hexadecimal | Descrição |
| - | - |
| FFFF | A sequência do dado chegou ao fim. |
| FFFE | O espaço está vazio. |
| Qualquer outro valor | Próxima posição da sequência do arquivo. |

Como utilizamos 16 bits para endereçamento dos setores, o nosso disco pode ter até 65536 setores independentemente da capacidade do disco. Visto que a tabela de alocação de arquivos aloca até 65536 linhas e que cada linha tem 2 bytes, é fato que a tabela de alocação de dados sempre terá 131072 bytes. Dessa forma, para descobrir quantos setores a tabela de alocação de dados ocupa, é necessário dividir 131072 pela quantidade de bytes por setor, que está especificado no Boot Record.

Vale lembrar que a tabela de alocação de arquivos sempre terá 65536 linhas, mas muitas vezes existirão linhas que endereçarão setores inexistentes, pois o disco pode ter menos do que 65536 setores. Dessa forma no Boot Record, é definido a quantidade de setores no disco. Assim, a tabela de alocação de arquivos não poderá utilizar a partir da posição correspondente ao último valor da tabela do Boot Record. 
A posição zero da tabela RCB representa a posição ( 1 + Tamanho da tabela RCB ) em setores. Em tempo, a posição zero sempre irá representar o primeiro setor logo após o diretório raiz.

## Tabela de Dados de Diretórios

Todos os diretórios da partição terá a seguinte tabela para cada entrada.

| Offset (em bytes) | Tamanho (em bytes) | Descrição |
| - | - | - |
| 0 | 24 | Nome do arquivo. |
| 25 | 1 | Atributo do arquivo, de acordo com a tabela abaixo. |
| 26 | 2 | Identificação do primeiro cluster do arquivo. |
| 28 | 4 | Tamanho do arquivo em bytes. |

## Dados possíveis do sistema
| Descrição | Valor|
|-|-|
|Livre | 00000000 |
|Diretório| 00000001 |
|Hidden| 00000010 |
|Deletado| 00000100 |
|Arquivo| 00001000 |


O diretório raiz tem exatamente 512 entradas. Para representar o ponteiro exatamente após o diretório raiz, expressamos com ( 1 + Tamanho da tabela RCB ) + ( (512 * 32) / bytes por setor ), que seria o espaço que temos até o inicio do diretório raiz somado ao tamanho em setores do diretório raiz.


## Subdiretórios

Os sub diretórios ocuparão pelo menos um setor. Cada setor terá n entradas, em que n representa o tamanho de cada diretorio dividido por 32, que é o tamanho da tabela de dados de diretórios. Vale lembrar que o sistema é restringido a apenas dois níveis de diretórios.
