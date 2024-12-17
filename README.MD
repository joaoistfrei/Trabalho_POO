# ♟️ **Jogo de Xadrez em C++ usando a Raylib**

## 📌 **Descrição do Projeto**

Este projeto é um jogo de xadrez clássico desenvolvido em **C++** utilizando a biblioteca gráfica **raylib**. O objetivo é proporcionar uma experiência fluida e amigável de xadrez para dois jogadores.

---

## 🎮 **Funcionalidades**

✅ **Interface Gráfica Simples**: Desenvolvido usando a raylib para renderização.  
✅ **Movimentos Válidos**: Posições válidas são destacadas, tornando o jogo um ambiente amigável para iniciantes.  
✅ **Detecção de Xeque e Xeque-mate**: Algoritmos implementados para detectar o fim do jogo.  
✅ **Jogabilidade para 2 Jogadores**: Permite uma partida local de xadrez no mesmo dispositivo.  

---

## 🛠️ **Tecnologias Utilizadas**

- **Linguagem**: C++ (C++17)
- **Biblioteca Gráfica**: [Raylib](https://www.raylib.com/)
- **Compilador**: g++

---

## 💻 **Como Executar o Projeto**

### ⚙️ Pré-requisitos

1. Tenha o **C++** instalado (compilador como `g++`).
2. Baixe e instale a biblioteca gráfica **raylib**.

### 🔧 **Instalação da Raylib**

Caso você ainda não tenha a raylib instalada:

#### Linux:

```bash
sudo apt install cmake git build-essential
git clone https://github.com/raysan5/raylib.git
cd raylib/src
make PLATFORM=PLATFORM_DESKTOP
sudo make install
```

---

### 🚀 **Execução do Projeto**

1. Clone este repositório:
   ```bash
   git clone https://github.com/SeuUsuario/JogoDeXadrez.git
   cd JogoDeXadrez
   ```

2. Compile o código com o **g++**:
   ```bash
   g++ -o chess main.cpp -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
   ```

3. Execute o jogo:
   ```bash
   ./chess
   ```

---

## 👥 **Autores**

Este projeto foi desenvolvido por:

- **João Pedro Machado Medeiros** - nUSP: 14582462  
- **Lucas Kimo Costa**  - nUSP: 14591156
- **Murilo Cury Pontes**  - nUSP: 13830417


## 🚀 **Divirta-se Jogando!**

🎉 Obrigado por testar o projeto. Esperamos que você curta o jogo de xadrez! ♟️