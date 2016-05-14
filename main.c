#include "extlib.more.h"

int main() {
    Heap h = heapNew(EL_SHORT);

    short test[7] = {5, 1, 3, 10, 6, 4, 18};

    for(int i=0; i<7; i++)
        heapPush(h, test[i]);

    printf("Count : %d\n", heapCount(h));

    for(; heapCount(h)>0; heapPop(h))
        printf("Get : %d\n", heapGet(h, short));

    heapDel(h);

    return EXIT_SUCCESS;
}

int main6() {
    String str = stringNew();

    stringAppend(str, "Bonjour");
    stringAppendChar(str, ' ');

    String str2 = stringNew();
    stringAppend(str2, "c'est");
    stringAppendString(str, str2);
    stringDel(str2);

    stringAppendChar(str, ' ');
    stringAppend(str, "moi");

    stringAppend(str, " ");

    str2 = stringCopy(str);

    for(int i=0; i<10; i++) {
        //stringAppend(str2, stringCStr(str2)); //pas bon parce que le pointeur change lors de la réallocation dans append
        stringAppendString(str2, str2);
        printf("%s\n\n\n", stringCStr(str2));
    }

    //for(int i=0; i<10; i++)

    //printf("%s\n", stringCStr(str2));
    stringHeap(str2);
    printf("\n");

    stringDel(str2);


    /*str2 = stringCopy(str);

    for(int i=0; i<10; i++)
        stringAppendString(str2, str2);

    //for(int i=0; i<10; i++)

    printf("%s\n", stringCStr(str2));
    stringHeap(str2);
    printf("\n");

    stringDel(str2);*/

    /*printf("%s\n", stringCStr(str));
    stringHeap(str);
    printf("\n");

    str2 = stringSubString(str, 4, 9);

    printf("%s\n", stringCStr(str2));
    stringHeap(str2);
    printf("\n");

    stringTrim(str, 4, 9);

    printf("%s\n", stringCStr(str));
    stringHeap(str);
    printf("\n");*/

    stringDel(str);

    return EXIT_SUCCESS;
}

int main5() {
int nb=100;
    Queue queue = queueNew(EL_SHORT);

    short *vals = malloc(nb*sizeof(short));

    for(int i=0; i<nb; i++)
        vals[i]=(short)i;

    for(int i=0; i<nb; i++)
        //vectPushBack(vect,&vals[i] );
        queuePush(queue, vals[i]);

    free(vals);

    queueHeap(queue);

    while(!queueIsEmpty(queue)) {
        printf("%d\n", queueGet(queue, short));
        queuePop(queue);
    }

    queueDel(queue);

    return EXIT_SUCCESS;
}

int main4() {
int nb=100;
    Vect vect = vectNew(EL_SHORT);

    short *vals = malloc(nb*sizeof(short));

    for(int i=0; i<nb; i++)
        vals[i]=(short)i;

    for(int i=0; i<nb; i++)
        //vectPushBack(vect,&vals[i] );
        vectPushFront(vect, vals[i]);

    free(vals);

    vectHeap(vect);

    vectReshape(vect);

    vectHeap(vect);

    for(int i=0; i<nb; i++)
        printf("%d\n", vectGetAt(vect, short, i));

    vectSort(vect, EL_ASC);

    for(int i=0; i<nb; i++)
        printf("%d\n", vectGetAt(vect, short, i));

    for(int i=0; i<nb; i++)
        vectPopFront(vect);

    printf("%d\n", vectCount(vect));

    /*for(int i=0; i<5; i++)
        vectPopBack(vect);*/

    vectDel(vect);

    return EXIT_SUCCESS;
}

int main3() {
int nb=100;
    Sll list = sllNew(EL_SHORT);

    short *vals = malloc(nb*sizeof(short));

    for(int i=0; i<nb; i++)
        vals[i]=(short)i;

    for(int i=0; i<nb; i++)
        sllPushBack(list, vals[i]);

    free(vals);

    sllHeap(list);

    for(int i=0; i<nb; i++)
        printf("%d\n", sllGetData(sllGetAt(list, i), short));

    sllSort(list, 0);

    for(int i=0; i<nb; i++)
        printf("%d\n", sllGetData(sllGetAt(list, i), short));

    /*for(int i=0; i<5; i++)
        vectPopBack(vect);*/

    sllDel(list);

    return EXIT_SUCCESS;
}

int main2() {
int nb=100;
    Dll list = dllNew(EL_SHORT);

    short *vals = malloc(nb*sizeof(short));

    for(int i=0; i<nb; i++)
        vals[i]=(short)i;

    for(int i=0; i<nb; i++)
        dllPushBack(list, vals[nb-1-i]);

    free(vals);

    dllHeap(list);

    for(int i=0; i<nb; i++)
        printf("%d\n", dllGetData(dllGetAt(list, i), short));

    dllSort(list, EL_ASC);

    for(int i=0; i<nb; i++)
        printf("%d\n", dllGetData(dllGetAt(list, i), short));

    /*for(int i=0; i<5; i++)
        vectPopBack(vect);*/

    dllDel(list);

    return EXIT_SUCCESS;
}
