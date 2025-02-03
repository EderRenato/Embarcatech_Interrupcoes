# Embarcatech_Interrupcoes
Repositório criado para a tarefa de Microcontroladores - Interrupções e Debounce

Desenvolvedor:

*Eder Renato da Silva Cardoso Casar*

# Instruções de compilação

Para compilar o código, são necessárias as seguintes extensões: 

*Plataforma de Desenvolvimento BitDogLab v6.3*

*Cmake*

*Wokwi Simulator* (Opcional, apenas se quiser ver a simulação)

Após instalá-las basta buildar o projeto pelo CMake. A partir daí, abra o arquivo 
diagram.json e clique no botão verde para iniciar a simulação, caso queira verificar antes de aplicar na placa.

Após isso, basta conectar a BitDogLab em modo BOOTSEL e dar run.

Enquanto na simulação, o usuário pode clicar nos botões vermelho e preto da simulação
a fim de, respectivamente, incrementar e decrementar o numero na matriz de led.

Na placa, o usuário pode clicar nos botões A e B para produzir o respectivo efeito de decremento e incremento.
Nos botões foram adicionados debounce via software para evitar leituras erradas.

Ao mesmo tempo em que o display númerico funciona, um led vermelho conectado a porta 13 do Raspberry Pi Pico W pisca
5 vezes por segundo através de funções de interrupção.

# Vídeo demonstrativo

https://youtu.be/0n9-R8NgBBc?si=EoU4KTWAyIjTyiDP
