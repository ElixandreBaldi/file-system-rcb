# RCB File System

Para utilizar o sistema de arquivos RCB é necessário utilizar o software de acesso do mesmo. O software consiste das seguintes funcionalidades:

- Formatador de dispositivo/partição.

- Transferência de arquivos de/para o RCBFS.

- Navegação nos arquivos do RCBFS.

- Exclusão de arquivos do RCBFS.

Entre outros...

## Formatando um dispositivo

Para formatar um dispositivo é necessário executar o seguinte comando como super usuário (`root`):

```
sudo rcbfs --format [dispositivo]
```

Substituindo `[dispositivo]` pelo caminho para o dispositivo no sistema.

> Dica: para encontrar um dispositivo no sistema digite `lsblk -o NAME,TYPE,SIZE,MODEL`. Prefixe o caminho com `/dev/`.

Exemplo:

```
sudo rcbfs -f /dev/sdc1
```

> Dica: você pode substituir comandos verbosos de traço duplo pelos atalhos de traço simples. Em vez de `--format`, basta utilizar `-f`.

Ao executar o comando, serão exibidos três diálogos:

- O usuário deseja realmente formatar o dispositivo? É necessário digitar `Y` para prosseguir ou qualquer outra tecla para abortar.

- O usuário deseja formatar o dispositivo de forma completa, ou seja, apagando realmente os dados do dispositivo? Este processo é bastante demorado. Novamente é necessário digitar `Y` para prosseguir ou qualquer outra tecla para abortar.

- Qual o tamanho do setor (em bytes) no dispositivo? É necessário informar um valor que seja potência de dois e maior ou igual a 16, que é o equivalente ao `boot record` do dispositivo. Se for informado um valor que não é potência de dois, será atribuido automaticamente um valor inferior que o seja. Além disso, se for informado um valor menor do que 16, será atribuído 16.

## Copiando um arquivo para o dispositivo

Para copiar um arquivo para o dispositivo basta indicar o arquivo desejado e o dispositivo destino. O arquivo será copiado para a raiz do dispositivo. O comando é o que segue:

```
sudo rcbfs --copy [arquivo] [dispositivo]
```

Substituindo `[arquivo]` pelo arquivo desejado e `[dispositivo]` pelo caminho do dispositivo.

Se o comando for executado com sucesso, nenhuma mensagem é exibida em tela. Em caso de erro, o mesmo é informado. Alguns dos possíveis erros podem ser:

- O arquivo ou dispositivo não existe ou o usuário não tem permissão para acessá-lo.

- O dispositivo não é um dispositivo RCBFS válido (é necessário formatá-lo previamente).

- O arquivo é grande demais para a capacidade disponível no dispositivo.

Exemplo:

```
sudo rcbfs -c /home/administrador/arquivo.php /dev/sdc1
```

## Acessando o dispositivo

Acessar o dispositivo é muito simples e comporta diversas funcionalidades. Para acessar um dispositivo:

```
sudo rcbfs --enter [dispositivo]
```

Ao acessar o dispositivo será exibido um indicador de diálogo `rcbfs>`, que indica que o dispositivo foi acessado com êxito e está aguardando comandos. Contudo o comando de acesso pode incidir em erro se:

- O dispositivo não existe ou o usuário não tem permissão para acessá-lo.

- O dispositivo não é um dispositivo RCBFS válido (é necessário formatá-lo previamente).

Se não houver erro, o dispositivo está aberto e preparado para novos comandos. Os comandos internos possuem uma breve semelhança com os comandos Unix. São eles:

### Ajuda
