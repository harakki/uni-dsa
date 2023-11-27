#include <SFML/Graphics.hpp>
#include <ctime>
#include <iostream>

#define TREE_SIZE 100

using std::cout;

struct Tree
{
    int data;

    Tree *left;
    Tree *right;

    Tree(int data) : data(data), left(nullptr), right(nullptr)
    {
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

void array_create_random(int array[], int size)
{
    for (int i = 0; i < size; ++i)
    {
        array[i] = i;
    }

    for (int i = 0; i < size; ++i)
    {
        int randomId = rand() % size;
        int temp = array[i];

        array[i] = array[randomId];
        array[randomId] = temp;
    }
}

void tree_sdp_insert_recursion(Tree *&root, int data)
{
    if (root == nullptr)
    {
        root = new Tree(data);
    }
    else if (data < root->data)
    {
        tree_sdp_insert_recursion(root->left, data);
    }
    else if (data > root->data)
    {
        tree_sdp_insert_recursion(root->right, data);
    }
    else
    {
        return;
    }
}

void tree_sdp_double_cosv(Tree **root, int data)
{
    Tree **p = root;

    while (*p != nullptr)
    {
        if (data < (*p)->data)
        {
            p = &((*p)->left);
        }
        else if (data > (*p)->data)
        {
            p = &((*p)->right);
        }
        else
        {
            return;
        }
    }

    *p = new Tree(data);
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
    else
    {
        int count_left = tree_size(node->left);
        int count_right = tree_size(node->right);

        return 1 + count_left + count_right;
    }
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
    srand((unsigned int)time(NULL));

    int i = 0;

    Tree *root_recursion = nullptr;
    Tree *root_double_cosv = nullptr;

    int gen_array[TREE_SIZE];
    int array_recursion[TREE_SIZE];
    int array_double_cosv[TREE_SIZE];

    array_create_random(gen_array, TREE_SIZE);

    for (int j = 0; j < TREE_SIZE; ++j)
    {
        array_recursion[j] = gen_array[j];
        array_double_cosv[j] = gen_array[j];
    }

    std::cout << "Случайное дерево поиска\n\n";
    cout << "Рекурсивная функция"
         << "\n";
    for (int i : array_recursion)
    {
        tree_sdp_insert_recursion(root_recursion, i);
    }
    std::cout << "\nОбход дерева cлева направо:\n";
    left_to_right(root_recursion);
    cout << "\nРазмер дерева: " << tree_size(root_recursion) << "\n";
    cout << "Контрольная сумма: " << tree_checksum(root_recursion) << "\n";
    cout << "Высота: " << tree_height(root_recursion) << "\n";
    cout << "Средняя высота: " << tree_middle_height(root_recursion) << "\n";

    cout << "\n\nФункция с двойной косвенностью"
         << "\n";
    for (int i : array_double_cosv)
    {
        tree_sdp_double_cosv(&root_double_cosv, i);
    }
    std::cout << "\nОбход дерева cлева направо:\n";
    left_to_right(root_recursion);
    cout << "\nРазмер дерева: " << tree_size(root_double_cosv) << "\n";
    cout << "Контрольная сумма: " << tree_checksum(root_double_cosv) << "\n";
    cout << "Высота: " << tree_height(root_double_cosv) << "\n";
    cout << "Средняя высота: " << tree_middle_height(root_double_cosv) << "\n";

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
        SFML_draw_tree(window, root_double_cosv, 1900);

        window.display();
    }

#endif

    return 0;
}