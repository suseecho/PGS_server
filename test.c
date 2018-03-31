#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include "cJSON.h"

pthread_mutex_t  mux;
pthread_t        thread_1;    // get weather infomation
pthread_t        thread_2;   //
char * temperature;
char * weather_text;

void *  thread1()
{
    typedef struct
    {
        char * name;
        char * country;
        char * timezone;
        char * text;      //weather
        char * temp;          //temperature
        char * last_update;
    }RESULT;
    FILE * fp;
    char buff[1024];
    char weather[1024] = "";
    if ((fp=fopen("weather.json","r")) == 0)
    {
        fprintf(stderr, "%s\n", "open json file failed");
    }
    while(fgets(buff,1024,fp)!=NULL)
    {
        strcat(weather,buff);
    }
    RESULT result;
    cJSON * root = cJSON_Parse(weather);
    cJSON * result_json = cJSON_GetObjectItem(root,"results");          //result_json is an array
    cJSON * item;
    char * p = NULL;
    int array_size = cJSON_GetArraySize(result_json);
    /*
    if(cJSON_IsArray(result_json))
    {
        printf("%d\n",array_size);
    }
    */
    for(int i = 0 ; i < array_size ; i++)
    {
        item = cJSON_GetArrayItem(result_json,i);
        p = cJSON_PrintUnformatted(item);
//        printf("%s\n",p);
    }
    cJSON * res = cJSON_Parse(p);
    cJSON * location_json = cJSON_GetObjectItem(res,"location");
    cJSON * now_json = cJSON_GetObjectItem(res,"now");
    result.text = cJSON_GetObjectItem(now_json,"text")->valuestring;
    result.country = cJSON_GetObjectItem(location_json,"country")->valuestring;
    result.last_update = cJSON_GetObjectItem(res,"last_update")->valuestring;
    result.name = cJSON_GetObjectItem(location_json,"name")->valuestring;
    result.temp = cJSON_GetObjectItem(now_json,"temperature")->valuestring;


    printf("location:%s\n",result.name);
    printf("counter:%s\n",result.country);
    printf("weather:%s\n",result.text);
    printf("temperature:%s\n",result.temp);
    printf("last update:%s\n",result.last_update);


    if(root)
        cJSON_Delete(root);
    fclose(fp);
    pthread_exit(NULL);
}

void thread_create()
{
    memset(&thread_1,0,sizeof(thread1));
    if((pthread_create(&thread_1,NULL,thread1,NULL)!=0))
        printf("create thread failed\n");
}

int main(int argc, char **args)
{
    thread_create();
    pthread_join(thread_1,NULL);
    return 0;
}
