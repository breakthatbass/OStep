
// only one thread can call this
void zem_init(zem_t *s, int value)
{
    s->value = value;
    cond_init(&s->cond);
    mutex_init(&s->lock);
}

void zem_wait(zem_t *s)
{
    mutex_lock(&s->lock);
    while (s->value <= 0)
        cond_wait(&s->cond, &s->lock);
    s->value--;
    mutex_unlock(&s->unlock);
}


void zem_post(zem_t *s)
{
    mutex_lock(&s->lock);
    s->value++;
    cond_signal(&s->cond);
    mutex_unlock(&s->lock);
}
