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

void tree_avl_rotate_LL(Tree *&ptr)
{
    Tree *ptr_2 = ptr->left;

    ptr->balance_factor = 0;
    ptr_2->balance_factor = 0;
    ptr->left = ptr_2->right;
    ptr_2->right = ptr;

    ptr = ptr_2;
}

void tree_avl_rotate_LR(Tree *&ptr)
{
    Tree *ptr_2 = ptr->left;
    Tree *ptr_3 = ptr_2->right;

    if (ptr_3->balance_factor < 0)
    {
        ptr->balance_factor = 1;
    }
    else
    {
        ptr->balance_factor = 0;
    }

    if (ptr_3->balance_factor > 0)
    {
        ptr_2->balance_factor = -1;
    }
    else
    {
        ptr_2->balance_factor = 0;
    }

    ptr_3->balance_factor = 0;
    ptr_2->right = ptr_3->left;
    ptr->left = ptr_3->right;
    ptr_3->left = ptr_2;
    ptr_3->right = ptr;

    ptr = ptr_3;
}

void tree_avl_rotate_RR(Tree *&ptr)
{
    Tree *ptr_2 = ptr->right;

    ptr->balance_factor = 0;
    ptr_2->balance_factor = 0;
    ptr->right = ptr_2->left;
    ptr_2->left = ptr;

    ptr = ptr_2;
}

void tree_avl_rotate_RL(Tree *&ptr)
{
    Tree *ptr_2 = ptr->right;
    Tree *ptr_3 = ptr_2->left;

    if (ptr_3->balance_factor < 0)
    {
        ptr->balance_factor = 1;
    }
    else
    {
        ptr->balance_factor = 0;
    }

    if (ptr_3->balance_factor > 0)
    {
        ptr_2->balance_factor = -1;
    }
    else
    {
        ptr_2->balance_factor = 0;
    }

    ptr_3->balance_factor = 0;
    ptr_2->left = ptr_3->right;
    ptr->right = ptr_3->left;
    ptr_3->left = ptr;
    ptr_3->right = ptr_2;

    ptr = ptr_3;
}

void tree_avl_add(Tree *&ptr, int data, bool &growth)
{
    if (ptr == nullptr)
    {
        ptr = new Tree(data);
        growth = 1;
    }
    else if ((ptr->data) > data)
    {
        tree_avl_add(ptr->left, data, growth);

        if (growth == 1)
        {
            if ((ptr->balance_factor) > 0)
            {
                ptr->balance_factor = 0;
                growth = 0;
            }
            else if ((ptr->balance_factor) == 0)
            {
                ptr->balance_factor = -1;
                growth = 1;
            }
            else
            {
                if (ptr->left->balance_factor < 0)
                {
                    tree_avl_rotate_LL(ptr);
                    growth = 0;
                }
                else
                {
                    tree_avl_rotate_LR(ptr);
                    growth = 0;
                }
            }
        }
    }
    else if ((ptr->data) < data)
    {
        tree_avl_add(ptr->right, data, growth);

        if (growth == 1)
        {
            if ((ptr->balance_factor) < 0)
            {
                ptr->balance_factor = 0;
                growth = 0;
            }
            else if ((ptr->balance_factor) == 0)
            {
                ptr->balance_factor = 1;
                growth = 1;
            }
            else
            {
                if (ptr->right->balance_factor > 0)
                {
                    tree_avl_rotate_RR(ptr);
                    growth = 0;
                }
                else
                {
                    tree_avl_rotate_RL(ptr);
                    growth = 0;
                }
            }
        }
    }
    else
    {
        cout << "Вершина уже в дереве";
    }
}

void tree_avl_rotate_LL1(Tree *&p, bool reduction)
{
    Tree *q = p->left;
    if (q->balance_factor == 0)
    {
        q->balance_factor = 1;
        p->balance_factor = -1;
        reduction = 0;
    }
    else
    {
        q->balance_factor = 0;
        p->balance_factor = 0;
    }
    p->left = q->right;
    q->right = p;
    p = q;
}

void tree_avl_rotate_RR1(Tree *&p, bool reduction)
{
    Tree *q = p->right;
    if (q->balance_factor == 0)
    {
        q->balance_factor = -1;
        p->balance_factor = 1;
        reduction = 0;
    }
    else
    {
        q->balance_factor = 0;
        p->balance_factor = 0;
    }
    p->right = q->left;
    q->left = p;
    p = q;
}

void balance_right_tree(Tree *&ptr, bool reduction)
{
    if (ptr->balance_factor == 1)
    {
        ptr->balance_factor = 0;
    }
    else if (ptr->balance_factor == 0)
    {
        ptr->balance_factor = -1;
        reduction = 0;
    }
    else if (ptr->balance_factor == -1)
    {
        if (ptr->left->balance_factor <= 0)
        {
            tree_avl_rotate_LL1(ptr, reduction);
        }
        else
        {
            tree_avl_rotate_LR(ptr);
        }
    }
}

void balance_left_tree(Tree *&p, bool reduction)
{
    if (p->balance_factor == -1)
    {
        p->balance_factor = 0;
    }
    else if (p->balance_factor == 0)
    {
        p->balance_factor = 1;
        reduction = 0;
    }
    else if (p->balance_factor == 1)
    {
        if (p->right->balance_factor >= 0)
        {
            tree_avl_rotate_RR1(p, reduction);
        }
        else
        {
            tree_avl_rotate_RL(p);
        }
    }
}

void tree_avl_node_deletion(Tree *&q, Tree *&r, bool reduction)
{
    if (r->right != NULL)
    {
        tree_avl_node_deletion(q, r->right, reduction);
        if (reduction == 1)
        {
            balance_right_tree(r, reduction);
        }
    }
    else
    {
        q->data = r->data;
        q = r;
        r = r->left;
        reduction = 1;
    }
}

Tree *tree_avl_delete(Tree *&ptr, int data, bool reduction)
{
    if (ptr == NULL)
    {
        cout << "Вершины нет в дереве";
    }
    else if (data < ptr->data)
    {
        tree_avl_delete(ptr->left, data, reduction);
        if (reduction == 1)
        {
            balance_left_tree(ptr, reduction);
        }
    }
    else if (data > ptr->data)
    {
        tree_avl_delete(ptr->right, data, reduction);
        if (reduction == 1)
        {
            balance_right_tree(ptr, reduction);
        }
    }
    else
    {
        Tree *q = ptr;
        if (q->left == NULL)
        {
            ptr = q->right;
            reduction = 1;
        }
        else if (q->right == NULL) 
        {
            ptr = q->left;
            reduction = 1;
        }
        else
        {
            tree_avl_node_deletion(q, q->left, reduction);
            if (reduction == 1)
            {
                balance_left_tree(ptr, reduction);
            }
        }
        delete q;
    }
    return ptr;
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

    int gen_array[TREE_SIZE];
    int input_delete = 0;
    array_create_inc(gen_array);

    bool growth = 0;
    bool reduction = 0;

    for (int i : gen_array)
    {
        tree_avl_add(root, i, growth);
    }

    std::cout << "АВЛ-дерево - удаление вершин"
              << "\n\n";

    left_to_right(root);

#ifdef SFML_GRAPHICS_HPP

    sf::RenderWindow window(sf::VideoMode(1900, 800), "I want pizza ;-;");

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

            tree_avl_delete(root, input_delete, reduction);

            cout << "\nПосле удаления вершины:\n";
            left_to_right(root);
            cout << "Средняя высота: " << tree_middle_height(root) << "\n";
        }

    }

#else

    for (int i = 0; i < TREE_SIZE; ++i)
    {
        cout << "\nВведите данные для удаления: ";
        cin >> input_delete;

        tree_avl_delete(root, input_delete, reduction);

        cout << "\nПосле удаления вершины:\n";
        left_to_right(root);
    }

#endif


    return 0;
}