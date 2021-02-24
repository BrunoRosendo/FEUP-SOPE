**Procedimento para contribuir para o projeto:**

1- Criar um branch com o nome da feature que estão a fazer (ou com o vosso nome).

2- Trabalhar nesse branch, fazendo commits regulares em partes essenciais da feature.

3- Quando acabarem, façam push do branch e pull request no Github (não façam já merge no Github).

Se houver mudanças no master, façam pull do vosso master local e façam rebase/merge para o vosso branch. Tratem dos conflitos localmente!

4- Esperem que outro elemento do grupo veja as mudanças. Esse elemento (ou mais do que um) pode pedir mudanças no código, seja para corrigir bugs ou para que algo possa ficar melhor.
É mais fácil fazer isto no Github do que no chat, já que podem referenciar linhas, ficheiros etc. Mas façam como quiserem

5- Quando a pessoa que deu review achar que está tudo bem, ela poderá fazer merge para o master.

****


Na linha de comandos, é mais ou menos isto. Antes de tudo, façam clone para o PC:
```
git clone https://github.com/BrunoRosendo/FEUP-SOPE.git
```

Para trabalhar localmente:
```
git checkout -b new-feature
git add .
git commit -m "Feature is done"
```

Se houver mudanças no master, fazer:
```
git checkout master
git pull
git checkout new-feature
git rebase master (para tratar de conflitos, sigam as instruções da command line)
```

Enviar para o Github:
```
git push --set-upstream origin new-feature (o set-upstream é so para depois poderem apenas fazer git pull)
```


Notas:

- Não adicionem ficheiros por upload. Isso escreve por cima e não verifica conflitos nem dá para checarmos o código
