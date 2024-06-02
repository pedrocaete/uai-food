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
    char nome[101];
} Cliente;

typedef struct {
    int codigo;
    char nome[MAX_DESCRICAO_PRATO];
} Restaurante;


typedef struct{
    int codigo;
    char restaurante[MAX_DESCRICAO_PRATO],nome[MAX_DESCRICAO_PRATO];
    int qtd;
    float preco;
}Pedidos;

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

Restaurante* ler_arq_rests(FILE **arq_rests,int *tam){
    Restaurante *restaurantes=NULL,aux;
    (*arq_rests)=fopen("restaurantes.bin","rb");
    
    if((*arq_rests)==NULL){
        perror("Erro ao abrir arquivo");
        return NULL;
        //Return porque se os arquivos não abriram o programa não irá funcionar
    }

    while(fread(&aux,sizeof(Restaurante),1,(*arq_rests))){
        (*tam)++;
        restaurantes=(Restaurante*) realloc(restaurantes,(*tam)*sizeof(Restaurante));
        restaurantes[(*tam)-1]=aux;
        if (restaurantes == NULL) {
            perror("Erro na realocação de memória para clientes.\n");
            return NULL;
        }
    }
    fclose(*arq_rests);
    return restaurantes;
}

Prato* ler_arq_pratos(FILE **arq_pratos,int *tam){
    Prato *pratos=NULL,aux;
    (*arq_pratos)=fopen("pratos.bin","rb");

    if((*arq_pratos)==NULL){
        perror("Erro ao abrir arquivo");
        return NULL;
        //Return porque se os arquivos não abriram o programa não irá funcionar
    }

    while(fread(&aux,sizeof(Prato),1,(*arq_pratos))){
        (*tam)++;
        pratos=(Prato*) realloc(pratos,(*tam)*sizeof(Prato));
        pratos[(*tam)-1]=aux;
        if (pratos == NULL) {
            perror("Erro na realocação de memória para clientes.\n");
            return NULL;
        }
    }
    fclose(*arq_pratos);
    return pratos;
}

Cliente* ler_arq_clientes(FILE **arq_clientes,int *tam){
    Cliente *clientes=NULL,aux;
    (*arq_clientes)=fopen("usuarios.bin","rb");

    if((*arq_clientes)==NULL){
        perror("Erro ao abrir arquivo");
        return NULL;
        //Return porque se os arquivos não abriram o programa não irá funcionar
    }

    while(fread(&aux,sizeof(Cliente),1,(*arq_clientes))){
        (*tam)++;
        clientes=(Cliente*) realloc(clientes,(*tam)*sizeof(Cliente));
        clientes[(*tam)-1]=aux;
        if (clientes == NULL) {
            perror("Erro na realocação de memória para clientes.\n");
            return NULL;
        }
    }
    fclose(*arq_clientes);
    return clientes;
}

Pedidos* ler_arq_pedidos(FILE **arq_pedidos,int *tam){
    Pedidos *pedidos=NULL,aux;
    (*arq_pedidos)=fopen("pedidos.bin","rb");

    if((*arq_pedidos)==NULL){
        perror("Erro ao abrir arquivo");
        return NULL;
        //Return porque se os arquivos não abriram o programa não irá funcionar
    }

    while(fread(&aux,sizeof(Pedidos),1,(*arq_pedidos))){
        (*tam)++;
        pedidos=(Pedidos*) realloc(pedidos,(*tam)*sizeof(Pedidos));
        pedidos[(*tam)-1]=aux;
        if (pedidos == NULL) {
            perror("Erro na realocação de memória para clientes.\n");
            return NULL;
        }
    }
    fclose(*arq_pedidos);
    return pedidos;
}

int escrever_arq_clientes(FILE *arq_clientes,Cliente *clientes,int total_clientes){
    arq_clientes=fopen("usuarios.bin","wb");

    if(arq_clientes==NULL){
        perror("Erro ao abrir arquivo");
        return 1;
        //Return porque se os arquivos não abriram o programa não irá funcionar
    }

    for (int i = 0; i < total_clientes; i++){
        if(clientes[i].codigo!=-1)
            if(fwrite(&clientes[i],sizeof(Cliente),1,arq_clientes)!=1){
                perror("Erro ao gravar no arquivo usuarios.bin");
                return 1;
        }
    }
    
    fclose(arq_clientes);
    return 0;
}

int escrever_arq_pedidos(FILE *arq_pedidos,Pedidos *pedidos,int total_pedidos){
    arq_pedidos=fopen("pedidos.bin","wb");

    if(arq_pedidos==NULL){
        perror("Erro ao abrir arquivo");
        return 1;
        //Return porque se os arquivos não abriram o programa não irá funcionar
    }

    for (int i = 0; i < total_pedidos; i++){
        if(fwrite(&pedidos[i],sizeof(Pedidos),1,arq_pedidos)!=1){
            perror("Erro ao gravar no arquivo pedidos.bin");
            return 1;   
        }
    }
    
    fclose(arq_pedidos);
    return 0;
}

void cadastrocliente(Cliente **clientes, int *total_clientes) {
    int opcao,maiorcod=0;
    if((*total_clientes)>0){
            for(int i=0;i<(*total_clientes);i++){
                if((*clientes)[i].codigo>maiorcod)
                    maiorcod=(*clientes)[i].codigo;
            }
        }
    do{
        maiorcod+=1;
        (*total_clientes)++;
        *clientes = (Cliente *) realloc(*clientes, (*total_clientes) * sizeof(Cliente));
        
        if (*clientes == NULL) {
            printf("Erro na realocação de memória para clientes.\n");
            return;
        }
        getchar();
        printf("Cadastre o nome do cliente:\n");
        fgets((*clientes)[(*total_clientes)-1].nome,100,stdin);
        (*clientes)[(*total_clientes)-1].nome[strlen((*clientes)[(*total_clientes)-1].nome)-1]='\0';
        
            //Para colocar códigos consecutivos nos clientes, começando a partir de 1
        (*clientes)[(*total_clientes)-1].codigo=maiorcod;
        printf("Deseja digitar mais um cliente? (1 para Sim, 0 para Não): ");
        scanf("%d", &opcao);
    }while(opcao);
}

void renomear_cliente(Cliente *clientes){
    getchar();
    printf("Digite o novo nome do cliente:\n");
    fgets((*clientes).nome,100,stdin);
    (*clientes).nome[strlen((*clientes).nome)-1]='\0';
}

void deletar_cliente(Cliente *clientes){
    int opc;
    printf("Tem certeza de que deseja deletar o cliente %s? Se sim digite 1:\n",(*clientes).nome);
    scanf("%d",&opc);
    if(opc==1){
        (*clientes).codigo=-1;
    }
}

void Resumo_Pedido(Pedidos *pedidos, Cliente *clientes, int num_pedidos, int cod) {
    int entrega = 0, pagamento;
    float preco_total = 0;
    char formatacao[151];
    sprintf(formatacao, "Resumo do Pedido - %s", clientes[0].nome);
    interface(formatacao);
    int vf = 0;
    for (int i = 0; i < num_pedidos; i++) {

        if ((i == 0 || strcmp(pedidos[i].restaurante, pedidos[i - 1].restaurante) != 0) && pedidos[i].codigo == cod) {
            printf("\n%s\n", pedidos[i].restaurante);
            entrega += 1;
            vf = 1;
        }
        if (vf == 1 && pedidos[i].codigo == cod) {
            printf("%dx %s = %.2f\n", pedidos[i].qtd, pedidos[i].nome, pedidos[i].qtd * pedidos[i].preco);
            preco_total += pedidos[i].qtd * pedidos[i].preco;
        }
    }
    sprintf(formatacao, "Taxa de entrega x%d = %.2f\nTOTAL DO PEDIDO = %.2f", entrega, (float)entrega * 4.99, preco_total + (float)entrega * 4.99);
    interface(formatacao);
    printf("Selecione a forma de pagamento:\n1-Pix\n2-Dinheiro\n3-Cartão de Crédito\n4-Cartão de Débito\n");
    scanf("%d", &pagamento);
}

void historico_pedidos(Cliente clientes,int total_pedidos,Pedidos *pedidos){
    char formatacao[151];
    int cod;
    sprintf(formatacao,"Histórico de pedidos - %s",clientes.nome);
    interface(formatacao);
    for(int j=0;j<total_pedidos;j++){
        int entrega=0,preco_total=0;
        if(j==0)
            cod=pedidos[j].codigo;
        if(pedidos[j].codigo==cod){
            sprintf(formatacao,"Pedido %d",cod);
            interface(formatacao);
            int vf=0;
            for(int i=0;i<total_pedidos;i++){
        
                if((i==0||strcmp(pedidos[i].restaurante,pedidos[i-1].restaurante)!=0)&&pedidos[i].codigo==cod){
                    printf("\n%s\n",pedidos[i].restaurante);
                    entrega+=1;
                    vf=1;
                }
                if(vf==1&&pedidos[i].codigo==cod){
                    printf("%dx %s = %.2f\n",pedidos[i].qtd,pedidos[i].nome,pedidos[i].qtd*pedidos[i].preco);
                    preco_total+=pedidos[i].qtd*pedidos[i].preco;
                }
            }
            printf("Taxa de entrega x%d = %.2f\n",entrega,(float)entrega*4.99);
            printf("************************************************\n");
            printf("TOTAL DO PEDIDO = %.2f\n\n",preco_total+(float)entrega*4.99);
            cod++;
        }    
    }
}

void pedidosuaifood(Cliente *clientes, Restaurante *restaurantes, Prato *pratos,Pedidos **pedidos, int quantidadeRestaurantes,int total_clientes,int total_pratos,int *total_pedidos) {
int codigoCliente,opcao,prato_escolhido,opc,maiorcod=0;
char formatacao[151];
do{
    int arrayrestaurante=-1,arrayprato=-1,qtd=0,x=-1;
    interface("Uai Food Pedidos - Login");
    printf("Digite o código do cliente (ou um número negativo para sair):\n");
    scanf("%d", &codigoCliente);
    for (int i = 0; i <total_clientes; i++) {
        if (clientes[i].codigo == codigoCliente) {
            x = i;
            break;
        }
    }
    
    if (x==-1)
        printf("Código do cliente não encontrado.\n");
    else if(codigoCliente >=0){   
        sprintf(formatacao,"Uai Food - %s",clientes[x].nome);
        interface(formatacao);
        printf("1 - Renomear cliente\n2 - Deletar cliente\n3 - Fazer pedido\n4 - Mostrar histórico de pedidos\n-1 - Fazer Logout\n");
        scanf("%d",&opc);
        switch(opc){
            case 1:
                renomear_cliente(&clientes[x]);
                break;
            case 2:
                deletar_cliente(&clientes[x]);
                break;
            case 3:
                if((*total_pedidos)>1)
                    maiorcod=(*pedidos)[(*total_pedidos)-2].codigo+1;
                else if((*total_pedidos)==1)
                    maiorcod=2;
                else
                    maiorcod=1;
                    //Para colocar códigos consecutivos nos clientes, começando a partir de 1
                do{
                printf("Bem-vindo(a) %s - Selecione o restaurante\n", clientes[x].nome);
                for (int i = 0; i < quantidadeRestaurantes; i++) 
                    printf("%d - %s\n", restaurantes[i].codigo, restaurantes[i].nome);
                printf("0 - Fechar Pedido\n-1 - Logout\n");
                scanf("%d", &opcao);
                for(int i=0;i<quantidadeRestaurantes; i++){
                    if(opcao==restaurantes[i].codigo){
                        arrayrestaurante=i;
                        break;
                    }
                }
                if(opcao==0){
                    Resumo_Pedido(*pedidos,&clientes[x], *total_pedidos,maiorcod);
                    maiorcod++;
                }
                if(opcao>0){
                    if(arrayrestaurante==-1)
                        printf("Restaurante não encontrado");
                    else{
                        printf("%s - Pratos - Menu de Opções\n", restaurantes[arrayrestaurante].nome);
                        for(int i=0; i<total_pratos; i++)
                            if(restaurantes[arrayrestaurante].codigo==pratos[i].restaurante){
                                printf("%d - %s - Preço: R$ %.2f\n", pratos[i].codigo, pratos[i].descricao, pratos[i].preco);
                            }
                        printf("0 - Fechar Pedido\n-1 - Voltar ao menu anterior\n");
                        scanf("%d",&prato_escolhido);
                        for(int i=0;i<total_pratos; i++){
                            if(prato_escolhido==pratos[i].codigo&&restaurantes[arrayrestaurante].codigo==pratos[i].restaurante){
                                arrayprato=i;
                                break;
                            }
                        }
                        if(prato_escolhido==0){
                            Resumo_Pedido(*pedidos,&clientes[x], *total_pedidos,maiorcod);
                            maiorcod++;
                        }
                        if(prato_escolhido>0){
                            if(arrayprato==-1)
                                printf("Prato não encontrado");
                            else{
                                printf("%s R$%.2f. Qual a quantidade? ",pratos[arrayprato].descricao,pratos[arrayprato].preco);
                                scanf("%d",&qtd);
                                if(qtd>0){
                                    (*total_pedidos)++;
                                    (*pedidos)=(Pedidos*)realloc(*pedidos,(*total_pedidos)*sizeof(Pedidos));
                                    if ((*pedidos) == NULL) {
                                        perror("Erro na realocação de memória para pedidos.\n");
                                        return;
                                    }
                                    (*pedidos)[(*total_pedidos)-1].qtd=qtd;
                                    (*pedidos)[(*total_pedidos)-1].preco=pratos[arrayprato].preco;
                                    strcpy((*pedidos)[(*total_pedidos)-1].restaurante,restaurantes[arrayrestaurante].nome);
                                    strcpy((*pedidos)[(*total_pedidos)-1].nome,pratos[arrayprato].descricao);
                                    (*pedidos)[(*total_pedidos)-1].codigo=maiorcod;
                                }                      
                            }
                        }
                    
                    }
                }   
            }while(opcao!=-1);
            break;
            case 4:
                historico_pedidos(clientes[x],*total_pedidos,*pedidos);
                break;
        default:
            printf("Opção inexistente.Tente Novamente\n");
            break;
        }
        
    } 
}while(codigoCliente>=0);
}

int main(){
    setlocale(LC_ALL, "Portuguese");
    Prato *pratos=NULL;
    Restaurante *restaurantes=NULL;
    Cliente *clientes=NULL;
    Pedidos *pedidos=NULL;
    FILE *arq_pratos,*arq_rests,*arq_clientes,*arq_pedidos;
    int total_rests=0,total_pratos=0,total_clientes=0,total_pedidos=0,opcao;

    restaurantes=ler_arq_rests(&arq_rests,&total_rests);
    pratos=ler_arq_pratos(&arq_pratos,&total_pratos);
    clientes=ler_arq_clientes(&arq_clientes,&total_clientes);
    pedidos=ler_arq_pedidos(&arq_pedidos,&total_pedidos);

    interface("Uai Food - Pedidos");

    do{
        printf("Selecione a opção desejada:\n1 - Criar cliente\n2 - Fazer login\n-1 - Sair\n");
        scanf("%d",&opcao);
        switch (opcao){
        case 1:
            cadastrocliente(&clientes, &total_clientes);
            break;
        case 2:
            pedidosuaifood(clientes,restaurantes,pratos,&pedidos,total_rests,total_clientes,total_pratos,&total_pedidos);
            break;
        case -1:
            break;
        default:
            printf("Opção inexistente.Tente Novamente\n");
            break;
        }
    }while(opcao!=-1);
    

    escrever_arq_clientes(arq_clientes,clientes,total_clientes);
    escrever_arq_pedidos(arq_pedidos,pedidos,total_pedidos);

    free(restaurantes);
    free(pratos);
    free(clientes);
    free(pedidos);

    exit(0);
}