#include <SFML/Graphics.hpp>
#include <iostream>

#define TREE_SIZE 100

using std::cout;
using std::cin;

struct Tree
{
    int data;

    int balance_factor;

    Tree *left;
    Tree *right;

    Tree(int data) : data(data), balance_factor(0), left(nullptr), right(nullptr)
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

void array_create_inc(int array[])
{
    for (int i = 0; i < TREE_SIZE; ++i)
    {
        array[i] = i;
    }
}

int vr = 1, hr = 1;

void tree_DBD_add(int data, Tree *&ptr)
{
    if (ptr == NULL)
    {
        ptr = new Tree(data);
        vr = 1;
    }
    else if (ptr->data > data)
    {
        tree_DBD_add(data, ptr->left);
        if (vr == 1)
        {
            if (ptr->balance_factor == 0)
            {
                Tree *q = ptr->left;
                ptr->left = q->right;
                q->right = ptr;
                ptr = q;
                q->balance_factor = 1;
                vr = 0;
                hr = 1;
            }
            else
            {
                ptr->balance_factor = 0;
                vr = 1;
                hr = 0;
            }
        }
        else
        {
            hr = 0;
        }
    }
    else if (ptr->data < data)
    {
        tree_DBD_add(data, ptr->right);
        if (vr == 1)
        {
            ptr->balance_factor = 1;
            hr = 1;
            vr = 0;
        }
        else if (hr == 1)
        {
            if (ptr->balance_factor == 1)
            {
                Tree *ptr_2 = ptr->right;
                ptr->balance_factor = 0;
                ptr_2->balance_factor = 0;
                ptr->right = ptr_2->left;
                ptr_2->left = ptr;
                ptr = ptr_2;
                vr = 1;
                hr = 0;
            }
            else
            {
                hr = 0;
            }
        }
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

    srand(time(NULL));

    int gen_array[TREE_SIZE];
    array_create_inc(gen_array);

    bool growth = 0;

    for (int i : gen_array)
    {
        tree_DBD_add(i, root);
    }

    std::cout << "ДБД-дерево"
              << "\n\n";
    std::cout << "Обход дерева cлева направо:"
              << "\n";
    left_to_right(root);
    cout << "\n"
         << "Размер дерева: " << tree_size(root) << "\n";
    cout << "Контрольная сумма: " << tree_checksum(root) << "\n";
    cout << "Высота: " << tree_height(root) << "\n";
    cout << "Средняя высота: " << tree_middle_height(root) << "\n";

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