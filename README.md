# RCB
O RCB é um sistema de arquivos baseado na implementação original do File Allocation Table (FAT), que utiliza uma lista ligada como método de alocação. Dessa forma, cada arquivo é composto por uma lista ligada de blocos do disco, evitando assim, desperdícios de memória com fragmentação interna pois, uma vez que o arquivo é composto por diversos blocos que de alguma forma referenciam-se uns aos outros, é possivel que esses blocos sejam espalhados no disco. O RCB utiliza 8 bits para endereçar os blocos.

O sistema de arquivos RCB enxerga a área de armazenamento como um conjunto de blocos de tamanhos iguais. Os dois primeiros blocos são reservados reservados: o primeiro bloco é o Boot Record, que armazena metadados fundamentais para a localização de qualquer dado no disco. O segundo bloco armazena a tabela que mapeia os arquivos, ou seja, a lista ligada. A partir do terceiro bloco, é destinado para o armazenamento dos arquivos e diretórios. Como utilizamos 8 bits para endereçamento dos blocos, o nosso disco pode ter até 256 blocos, mais os dois blocos iniciais, ou seja, independentemente da capacidade do disco, ele será dividido em 258 blocos. Visto que a tabela de alocação de arquivos aloca até 256 linhas e que cada linha tem 1 byte, é ideal que cada bloco tenha no mínimo 256 bytes, neste caso, o disco teria no mínimo 66048 bytes de capacidade.

## Boot Record

Como dito anteriormente, o boot record é o primeiro bloco do sistema de arquivos. Aqui temos metadados que servirão como referências para a localização de qualquer informação no disco. Um exemplo seria o endereço físico dos limites dos blocos.
Abaixo, temos uma tabela indicando o significado de cada metadado, o offset de início dos mesmos juntamente com o tamanho, ambos parametros informados a nível de bytes.

| Offset (em bytes) | Tamanho (em bytes) | Descrição |
| -------------- | --------------- | --------- |
| 0 | 2 | Assinatura do sistema de arquivos RCB |
|   | 2 | Quantidade de bytes por blocos |
|   | 2 | Quantidade de blocos reservados|
