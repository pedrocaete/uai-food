#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <math.h>
#include <stdlib.h>

#define MAX_DESCRICAO_PRATO 101
typedef struct {
    int restaurante;
    int codigo;
    char descricao[MAX_DESCRICAO_PRATO];
    float preco;
} Prato;

typedef struct {
    int codigo;
    char nome[MAX_DESCRICAO_PRATO];
} Restaurante;

void interface(char *conteudo);
void Renomear_Restaurante(Restaurante *restaurantes,FILE *arq_rests);
int Deletar_Restaurante(Restaurante *restaurantes,FILE *arq_rests);
void Renomear_Prato(Prato *pratos,FILE *arq_pratos);
void Alterar_Preco_Prato(Prato *prato,FILE *arq_pratos);
void Deletar_Prato(Prato *pratos,FILE *arq_pratos);
void Criar_Restaurante(FILE *arq_rests);
void Criar_Prato(FILE* arq_pratos,Restaurante restaurantes);
void Login(FILE *arq_rests,FILE *arq_pratos);

int main(){
    setlocale(LC_ALL, "Portuguese");
    FILE *arq_rests, *arq_pratos;
    arq_rests=fopen("restaurantes.bin","ab");
    arq_pratos=fopen("pratos.bin","ab");
    fclose(arq_rests);
    fclose(arq_pratos);
    arq_rests=fopen("restaurantes.bin","rb+");
    arq_pratos=fopen("pratos.bin","rb+");
    if(arq_rests==NULL||arq_pratos==NULL){
        perror("Erro ao abrir arquivos");
        return 1;
        //Return porque se os arquivos não abriram o programa não irá funcionar
    }
    int opcao;
    do{
        interface("Uai Food-Restaurantes");
        printf("Digite a ação desejada:\n1-Criar Restaurante\n2-Fazer Login\n1-Sair\n");
        scanf("%d",&opcao);
        switch(opcao){
            case 1:
                Criar_Restaurante(arq_rests);
                break;
            case 2:
                Login(arq_rests,arq_pratos);
                break;
            case-1:
                break;
            default:
                printf("Comando Inexistente. Tente Novamente\n");
        }
    }while(opcao!=-1);
    exit(0);
}

void interface(char *conteudo) {
    int moldura=100,centralizador,num_palavras;
    num_palavras=strlen(conteudo);
    centralizador=(moldura/2-num_palavras)/2;
    for(int i=0;i<moldura;i++){
        if(i<moldura/2)
            printf("*");
        if(i==moldura/2-1){
            printf("\n");
            for(int i=0;i<=centralizador;i++)
                printf(" ");
            printf("%s\n",conteudo);
        }
        if(i>=moldura/2)
            printf("*");
    }
    printf("\n\n\n");
}

void Renomear_Restaurante(Restaurante *restaurantes,FILE *arq_rests){
    Restaurante r;
    printf("Digite o novo nome do restaurante:\n");
    getchar();
    fgets(restaurantes->nome, MAX_DESCRICAO_PRATO, stdin);
    restaurantes->nome[strcspn(restaurantes->nome, "\n")] = '\0';
    rewind(arq_rests);
    while(fread(&r,sizeof(Restaurante),1,arq_rests)!=0)
        if(r.codigo==restaurantes->codigo){
            fseek(arq_rests, -sizeof(Restaurante), 1);
            if(fwrite(restaurantes,sizeof(Restaurante),1,arq_rests)==1)
                printf("Restaurante renomeado com sucesso!\n");
            else
                perror("Erro ao gravar dados no arquivo");
            break;
        }
   
}

int Deletar_Restaurante(Restaurante *restaurantes,FILE *arq_rests){
    int deletar;
    Restaurante r;
    printf("Tem certeza que deseja deletar o restaurante %s?\nDigite 1 para deletá-lo:\n",restaurantes->nome);
    scanf("%d",&deletar);
    if(deletar==1){
        rewind(arq_rests);
        while(fread(&r,sizeof(Restaurante),1,arq_rests)!=0)
            if(r.codigo==restaurantes->codigo){
                strcpy(restaurantes->nome,"\0");
                restaurantes->codigo=-1;
                fseek(arq_rests, -sizeof(Restaurante), 1);
                if(fwrite(restaurantes,sizeof(Restaurante),1,arq_rests)==1)
                    printf("Restaurante deletado com sucesso!\n");
                else
                    perror("Erro ao gravar dados no arquivo");
            break;
            }
    }
    return -1;
    //Return -1 para acabar com o loop em login após deletar restaurantes ser executado
}

void Renomear_Prato(Prato *pratos,FILE *arq_pratos) {
    Prato p;
    printf("Digite o novo nome para o prato: ");
    getchar();
    fgets(pratos->descricao, MAX_DESCRICAO_PRATO, stdin);
    pratos->descricao[strcspn(pratos->descricao, "\n")] = '\0';
    rewind(arq_pratos);
    while(fread(&p,sizeof(Prato),1,arq_pratos)!=0)
        if(p.codigo==pratos->codigo){
            fseek(arq_pratos, -sizeof(Prato), 1);
            if(fwrite(pratos,sizeof(Prato),1,arq_pratos)==1)
                printf("Prato renomeado com sucesso!\n");
            else
                perror("Erro ao gravar dados no arquivo");
            break;
        }
}

void Alterar_Preco_Prato(Prato *pratos,FILE *arq_pratos) {
    Prato p;
    printf("Digite o novo preco para o prato: ");
    scanf("%f", &pratos->preco);
    rewind(arq_pratos);
    while(fread(&p,sizeof(Prato),1,arq_pratos)!=0)
        if(p.codigo==pratos->codigo){
            fseek(arq_pratos, -sizeof(Prato), 1);
            if(fwrite(pratos,sizeof(Prato),1,arq_pratos)==1)
                printf("Preço alterado com sucesso!\n");
            else
                perror("Erro ao gravar dados no arquivo");
            break;
        }
}

void Deletar_Prato(Prato *pratos,FILE *arq_pratos){
    Prato p;
    rewind(arq_pratos);
    while(fread(&p,sizeof(Prato),1,arq_pratos)!=0)
        if(p.codigo==pratos->codigo){
            pratos->codigo = -1;
            pratos->preco = 0.0;
            strcpy(pratos->descricao, "");
            fseek(arq_pratos, -sizeof(Prato), 1);
            if(fwrite(pratos,sizeof(Prato),1,arq_pratos)==1)
                printf("Prato deletado com sucesso!\n");
            else
                perror("Erro ao gravar dados no arquivo");
            break;
        }
}

void Criar_Restaurante(FILE *arq_rests){
    Restaurante restaurantes;
    int codigo=0;
    interface("Uai Food-Criar Restaurante");
    rewind(arq_rests);
    while(fread(&restaurantes,sizeof(Restaurante),1,arq_rests)!=0)
        if(restaurantes.codigo>codigo)
            codigo=restaurantes.codigo;
    //Criado para descobrir qual o maior código de restaurante, pois o código do próxio restaurante criado tem de ser seu sucessor
    printf("Digite o nome do restaurante:\n");
    getchar();
    fgets(restaurantes.nome,MAX_DESCRICAO_PRATO-1,stdin);
    restaurantes.nome[strcspn(restaurantes.nome, "\n")] = '\0';
    restaurantes.codigo=codigo+1;
    if(fwrite(&restaurantes,sizeof(Restaurante),1,arq_rests)==1)
        printf("Restaurante criado com sucesso!\n");
    else
        perror("Erro ao criar restaurante\n");
}

void Criar_Prato(FILE* arq_pratos,Restaurante restaurantes){
    Prato pratos;
    char formatacao[MAX_DESCRICAO_PRATO+13];
    int codigo=0;
    rewind(arq_pratos);
    while(fread(&pratos,sizeof(Prato),1,arq_pratos)!=0)
        if(pratos.codigo>codigo&&restaurantes.codigo==pratos.restaurante)
            codigo=pratos.codigo;
    sprintf(formatacao,"%s - Novo Prato",restaurantes.nome);
    interface(formatacao);
    printf("Digite o nome do novo prato:\n");
    getchar();
    fgets(pratos.descricao,MAX_DESCRICAO_PRATO-1,stdin);
    pratos.descricao[strcspn(pratos.descricao,"\n")] = '\0';
    printf("Digite o novo preço do prato: ");
    scanf("%f",&pratos.preco);
    pratos.codigo=codigo+1;
    pratos.restaurante=restaurantes.codigo;
    fwrite(&pratos,sizeof(Prato),1,arq_pratos);
}

void Login(FILE *arq_rests,FILE *arq_pratos){
    int restaurante_desejado,menu_opcoes,prato_desejado,altera_prato,alteracao,vf;
    //vf foi criada para poder ser emitido o "erro, resutantente nao encontrado"
    Restaurante restaurantes;
    Prato pratos;
    char menu_formatado[MAX_DESCRICAO_PRATO+30];
    //Criada para colocar a estrutura de menu - pratos na funcao interface, pois ela só aceita strings formatadas
    interface("Uai Food-Login Restaurante");
    do{
    vf=0;
    printf("Digite o código do restaurante (ou um número negativo para sair):\n");
    scanf("%d",&restaurante_desejado);
    rewind(arq_rests);
    while(fread(&restaurantes,sizeof(Restaurante),1,arq_rests)!=0){
        if(restaurante_desejado==restaurantes.codigo&&restaurante_desejado>0){
            vf=1;
            do{
                sprintf(menu_formatado,"%s - Menu de Opções",restaurantes.nome);
                interface(menu_formatado);
                printf("1 - Renomear Restaurante\n2 - Deletar Restaurante\n3 - Mostrar pratos\n");
                printf("-1 - Logout\n");
                scanf("%d",&menu_opcoes);
                switch (menu_opcoes){
                    case 1:
                        Renomear_Restaurante(&restaurantes,arq_rests);
                        break;
                    case 2:
                        menu_opcoes=Deletar_Restaurante(&restaurantes,arq_rests);
                        break;
                    case 3:
                        sprintf(menu_formatado,"%s - Pratos - Menu de Opções",restaurantes.nome);
                        interface(menu_formatado);
                        rewind(arq_pratos);
                        while(fread(&pratos,sizeof(Prato),1,arq_pratos)!=0){
                            if(pratos.restaurante==restaurantes.codigo&&pratos.restaurante>=0)
                            printf("%d - %s - R$ %.2f\n",pratos.codigo,pratos.descricao,pratos.preco);
                        }
                        printf("0 - Cadastrar novo prato\n-1 - Voltar ao menu anterior\n");
                        scanf("%d",&prato_desejado);
                        rewind(arq_pratos);
                        while(fread(&pratos,sizeof(Prato),1,arq_pratos)!=0)
                        if(pratos.codigo==prato_desejado&&pratos.restaurante==restaurantes.codigo){
                            printf("Código do prato: %d\nCódigo do Restaurante: %d\nNome do Prato: %s\nPreço: R$ %.2f\n",pratos.codigo,pratos.restaurante,pratos.descricao,pratos.preco);
                            printf("Deseja fazer alguma alteração nos dados do prato? Se sim, digite 1:\n");                
                            scanf("%d", &alteracao);
                            if(alteracao==1){
                                printf("1 - Renomear\n2 - Alterar preço\n3 - Deletar\n-1 - Voltar ao menu anterior\n");
                                scanf("%d", &altera_prato);
                                switch(altera_prato){
                                case 1:
                                    Renomear_Prato(&pratos,arq_pratos);
                                    break;                        
                                case 2:
                                    Alterar_Preco_Prato(&pratos,arq_pratos);
                                    break;                        
                                case 3:
                                    Deletar_Prato(&pratos,arq_pratos);
                                    break;                        
                                }
                            }
                            break;
                        }
                        if(prato_desejado==0)
                        Criar_Prato(arq_pratos,restaurantes);
                        break;
                    default:
                        break;
                }
            }while(menu_opcoes!=-1);
        }
    }
        if(vf!=1)printf("Restaurante não encontrado. Tente novamente.\n");
    }while(restaurante_desejado>=0);

}
