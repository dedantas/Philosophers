# 🍝 Philosophers — 42 School

Simulação do clássico **dining philosophers problem**: N filósofos sentados à mesa, compartilhando garfos, tentando comer sem morrer de fome e sem causar deadlock. Implementado em C usando threads e mutexes, sem race conditions.

## 🧠 Conceitos aplicados
- Threads (`pthread_create` / `pthread_join`)
- Mutexes e exclusão mútua (`pthread_mutex_t`)
- Prevenção de deadlock e starvation
- Timing preciso com `gettimeofday` / `usleep`
- Gerenciamento de estado compartilhado entre threads

## ⚙️ Compilação
```bash
make            # compila o projeto
make debug      # compila com ThreadSanitizer (detecção de race conditions)
make clean      # remove .o
make fclean     # remove .o e o binário
make re         # recompila do zero
```

## ▶️ Uso
```bash
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```

Exemplos:
```bash
./philo 5 800 200 200        # 5 filósofos, sem limite de refeições
./philo 4 410 200 200 5      # 4 filósofos, cada um deve comer 5 vezes
./philo 1 800 200 200        # caso extremo: 1 filósofo (nunca come, sempre morre)
```

## 🏗️ Estrutura
```
src/
├── main.c        # parsing de argumentos e ponto de entrada
├── init.c        # inicialização das estruturas e mutexes
├── simulation.c  # loop principal da simulação
├── actions.c     # ações dos filósofos (comer, dormir, pensar)
├── utils.c       # funções auxiliares
└── cleanup.c     # liberação de recursos e mutexes
```

## 📌 Aprendizados
Este projeto foi meu primeiro contato sério com programação concorrente em C. O maior desafio foi evitar deadlocks ao pegar os garfos (ordem de aquisição) e manter timing preciso sem sobrecarregar a CPU.
