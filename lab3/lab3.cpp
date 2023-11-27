#include <SFML/Graphics.hpp>
#include <iostream>

#define TREE_SIZE 5

using std::cin;
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

void array_create_inc(int array[])
{
    for (int i = 0; i < TREE_SIZE; ++i)
    {
        array[i] = i;
    }
}

void tree_sdp_double_cosv_create(Tree **root, int data)
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

void tree_sdp_delete_node(Tree *&root, int data)
{
    Tree **first_ptr = &root;

    while (*first_ptr != NULL)
    {
        if (data < (*first_ptr)->data)
            first_ptr = &((*first_ptr)->left);
        else if (data > ((*first_ptr)->data))
            first_ptr = &((*first_ptr)->right);
        else
            break;
    }

    if (*first_ptr != NULL)
    {
        Tree *second_ptr = *first_ptr;

        if ((second_ptr->left) == NULL)
            *first_ptr = second_ptr->right;
        else if ((second_ptr->right) == NULL)
            *first_ptr = second_ptr->left;
        else
        {
            Tree *third_ptr = second_ptr->left;
            Tree *fouth_ptr = second_ptr;

            if (third_ptr->right == NULL)
            {
                third_ptr->right = second_ptr->right;
                *first_ptr = third_ptr;
            }
            else
            {
                while ((third_ptr->right) != NULL)
                {
                    fouth_ptr = third_ptr;
                    third_ptr = third_ptr->right;
                }
                fouth_ptr->right = third_ptr->left;
                third_ptr->left = second_ptr->left;
                third_ptr->right = second_ptr->right;
                *first_ptr = third_ptr;
            }
        }

        delete second_ptr;
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

int main()
{
    Tree *root = nullptr;

    int gen_array[TREE_SIZE];
    int input_delete = 0;

    array_create_inc(gen_array);

    for (int i : gen_array)
    {
        tree_sdp_double_cosv_create(&root, i);
    }

    std::cout << "Случайное дерево поиска - удаление вершин\n\n";

    left_to_right(root);

    #ifdef SFML_GRAPHICS_HPP

    sf::RenderWindow window(sf::VideoMode(1900, 800), "ok");

    while (window.isOpen())
    {
        for (int i = 0; i < TREE_SIZE; ++i)
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

            cout << "\nВведите данные для удаления: ";
            cin >> input_delete;

            tree_sdp_delete_node(root, input_delete);

            cout << "\nПосле удаления вершины:\n";
            left_to_right(root);
        }
    }

#else

    for (int i = 0; i < TREE_SIZE; ++i)
    {
        cout << "\nВведите данные для удаления: ";
        cin >> input_delete;

        tree_sdp_delete_node(root, input_delete);

        cout << "\nПосле удаления вершины:\n";
        left_to_right(root);
    }

#endif

    return 0;
}