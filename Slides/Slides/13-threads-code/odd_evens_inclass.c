int count = 0;
pthread_mutex_t count_mutex;
void *even_work(void *t){
  
  int my_count = 0;
  while(my_count < 5){
    pthread_mutex_lock(&count_mutex);
    if(count % 2 == 0){
      count++;
      my_count++;
    }
    pthread_mutex_unlock(&count_mutex);

  }

  int my_count = 0;
  while(my_count < 5){
    if(count % 2 == 0){
      // count changes
      pthread_mutex_lock(&count_mutex);
      count++;
      my_count++;
      pthread_mutex_unlock(&count_mutex);
    }

  }


}
