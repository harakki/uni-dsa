#include <SFML/Graphics.hpp>
#include <iostream>
#include <algorithm>

#define TREE_SIZE 100

using std::cin;
using std::cout;

struct Tree
{
    int data;
    int weight;

    Tree *left;
    Tree *right;

    Tree(int Data, int Weight)
    {
        data = Data;
        weight = Weight;

        left = nullptr;
        right = nullptr;
    }
};

#ifdef SFML_GRAPHICS_HPP

void SFML_draw_node(sf::RenderWindow &window, Tree *node, float x, float y, float offset_by_x)
{
    if (node != nullptr)
    {
        // Параметры круга
        sf::CircleShape node_outline(20);

        node_outline.setPosition(x, y);
        node_outline.setFillColor(sf::Color::Transparent);
        node_outline.setOutlineColor(sf::Color::Black);
        node_outline.setOutlineThickness(1);

        // Параметры текста
        sf::Font font;
        if (font.loadFromFile("sfml-resources\\Arial.ttf") == 0)
        {
            return;
        }

        sf::Text node_data(std::to_string(node->data), font, 20);
        node_data.setPosition(x + 10, y + 10);
        node_data.setFillColor(sf::Color::Black);

        // Отрисовываем ноды
        window.draw(node_outline);
        window.draw(node_data);

        if (node->left)
        {
            sf::Vertex line[] = {sf::Vertex(sf::Vector2f(x + 20, y + 40)),
                                 sf::Vertex(sf::Vector2f(x - offset_by_x + 38, y + 110))};
            // Параметры линий
            line[0].color = sf::Color::Black;
            line[1].color = sf::Color::Black;

            window.draw(line, 2, sf::Lines);

            SFML_draw_node(window, node->left, x - offset_by_x, y + 100, offset_by_x / 2);
        }
        if (node->right)
        {
            sf::Vertex line[] = {sf::Vertex(sf::Vector2f(x + 20, y + 40)),
                                 sf::Vertex(sf::Vector2f(x + offset_by_x + 2, y + 110))};
            // БАГФИКС ЭТИХ ДВУХ СТРОЧЕК ЗАНЯЛ ЧАС
            line[0].color = sf::Color::Black;
            line[1].color = sf::Color::Black;

            window.draw(line, 2, sf::Lines);

            SFML_draw_node(window, node->right, x + offset_by_x, y + 100, offset_by_x / 2);
        }
    }
}

void SFML_draw_tree(sf::RenderWindow &window, Tree *root, int window_width)
{
    if (root != nullptr)
    {
        // Начальные координаты корня дерева
        float x = window_width / (float)2;
        float y = 20;

        // Расстояние между узлами
        float offset_by_x = window_width / (float)4;

        SFML_draw_node(window, root, x, y, offset_by_x);
    }
}

#endif

int V[TREE_SIZE + 1];
int W[TREE_SIZE + 1];
int AW[TREE_SIZE + 1][TREE_SIZE + 1];
int AP[TREE_SIZE + 1][TREE_SIZE + 1];
int AR[TREE_SIZE + 1][TREE_SIZE + 1];

void array_create_inc()
{
    srand((unsigned int)time(NULL));
    V[0] = 0;
    W[0] = 0;
    for (int i = 0; i <= TREE_SIZE; i++)
    {
        V[i] = 1 + rand() % 1000000;
        W[i] = 1 + rand() % 10;
    }

    //std::sort(V, V + TREE_SIZE + 1);
    std::sort(W, W + TREE_SIZE + 1);
}

void fill_matrix_weight()
{
    for (int i = 0; i <= TREE_SIZE; i++)
    {
        for (int j = i + 1; j <= TREE_SIZE; j++)
        {
            AW[i][j] = AW[i][j - 1] + W[j];
        }
    }
}

void fill_matrix_AP_AR()
{
    int i, j, m, minVal, k, h, x;
    for (i = 0; i <= TREE_SIZE - 1; i++)
    {
        j = i + 1;
        AP[i][j] = AW[i][j];
        AR[i][j] = j;
    }
    for (h = 2; h <= TREE_SIZE; h++)
    {
        for (i = 0; i <= (TREE_SIZE - h); i++)
        {
            j = i + h;
            m = AR[i][j - 1];
            minVal = AP[i][m - 1] + AP[m][j];
            for (k = m + 1; k <= AR[i + 1][j]; k++)
            {
                x = AP[i][k - 1] + AP[k][j];
                if (x < minVal)
                {
                    m = k;
                    minVal = x;
                }
            }
            AP[i][j] = minVal + AW[i][j];
            AR[i][j] = m;
        }
    }
}

void add_to_optimal_search_tree(int Data, int Weight, Tree *&ptr)
{
    if (ptr == nullptr)
    {
        ptr = new Tree(Data, Weight);
    }
    else if (Data < (ptr->data))
    {
        add_to_optimal_search_tree(Data, Weight, ptr->left);
    }
    else if (Data > (ptr->data))
    {
        add_to_optimal_search_tree(Data, Weight, ptr->right);
    }
}

// Классический алгоритм
void create_optimal_search_tree(int Left, int Right, Tree *&root)
{
    if (Left < Right)
    {
        int k = AR[Left][Right];
        add_to_optimal_search_tree(V[k], W[k], root);
        create_optimal_search_tree(Left, k - 1, root);
        create_optimal_search_tree(k, Right, root);
    }
}

void left_to_right(Tree *node)
{
    if (node)
    {
        left_to_right(node->left);
        std::cout << node->data << " ⟩ ";
        left_to_right(node->right);
    }
}

int tree_size(Tree *node)
{
    if (node == nullptr)
    {
        return 0;
    }

    int count_left = tree_size(node->left);
    int count_right = tree_size(node->right);

    return 1 + count_left + count_right;
}

int tree_checksum(Tree *node)
{
    if (node == nullptr)
    {
        return 0;
    }

    int checksum_left = tree_checksum(node->left);
    int checksum_right = tree_checksum(node->right);

    return node->data + checksum_left + checksum_right;
}

float tree_height(Tree *node)
{
    if (node == nullptr)
    {
        return 0;
    }

    float height_left = tree_height(node->left);
    float height_right = tree_height(node->right);

    return std::max(height_left, height_right) + 1;
}

float tree_height_sum(Tree *node, int depth)
{
    if (node == nullptr)
    {
        return 0;
    }

    float sum_left = tree_height_sum(node->left, depth + 1);
    float sum_right = tree_height_sum(node->right, depth + 1);

    return depth + sum_left + sum_right;
}

float tree_middle_height(Tree *root)
{
    return (float)(tree_height_sum(root, 1) / tree_size(root));
}

int main()
{
    Tree *root = nullptr;

    array_create_inc();

    root = new Tree(V[1], W[1]);

    fill_matrix_weight();
    fill_matrix_AP_AR();
    create_optimal_search_tree(0, TREE_SIZE, root);

    cout << "ДОП-дерево"
         << "\n\n"
         << "Обход дерева cлева направо:"
         << "\n";

    left_to_right(root);

    cout << "\n"
         << "Размер дерева: " << tree_size(root) << "\n"
         << "Контрольная сумма: " << tree_checksum(root) << "\n"
         << "Средневзвешенная высота: " << tree_middle_height(root) << "\n";

#ifdef SFML_GRAPHICS_HPP

    sf::RenderWindow window(sf::VideoMode(1900, 800), "looks strange...");

    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::White);
        SFML_draw_tree(window, root, 1900);

        window.display();
    }

#endif

    return 0;
}