#include<iostream>
#include<vector>
#include<cmath>
#include<algorithm>
#include<typeinfo>
#include <type_traits>
#include "classes.h"
#include "glut.h"


using namespace std;

static int window;
static int menu_id;
static int submenu_id;
static int submenu_lines;
static int submenu_info;
static int value = 0;
vector<Point> ALL_POINTS ;
vector<Line> ALL_LINES ;
vector< unique_ptr<Shape> > ALL_SHAPES;

void print_edit_menu()
{
    cout << "Выберите, что вы хотите сделать с фигурвми: " << "\n";
    cout << "1. Поворот\n";
    cout << "2. Рефлекс ( точка )\n";
    cout << "3. Рефлекс ( линия )\n";
    cout << "4. Масштаб\n";
}

void print_all_shapes(int a = 1) {
    Polygon pol();
    cout << "Точки: (x, y)\n";
    for (int i = 0; i < ALL_POINTS.size(); i++) {
        cout << "  " << a + i << ". (" << ALL_POINTS[i].x << ", " << ALL_POINTS[i].y << ")\n";
    }
    a += ALL_POINTS.size();
    cout << "Линии:   k   b\n";
    for (int i = 0; i < ALL_LINES.size(); i++) {
        cout << "  " << a + i << ". " << ALL_LINES[i].k << ", " << ALL_LINES[i].b << '\n';
    }
    a += ALL_LINES.size();
    cout << "Фигуры: \n";
    for (int i = 0; i < ALL_SHAPES.size(); i++) {
        cout << "  " << a + i  << ". ";
        if (nullptr != dynamic_cast<Circle*>(ALL_SHAPES[i].get())) {
            cout << " КРУГ: " << ALL_SHAPES[i].get()->getInfo();
        }
        else if (nullptr != dynamic_cast<Ellipse*>(ALL_SHAPES[i].get())) {
            cout << " ЭЛЛИПС: " << ALL_SHAPES[i].get()->getInfo();
        }
        else if (nullptr != dynamic_cast<Square*>(ALL_SHAPES[i].get())) {
            cout << " КВАДРАТ: " << ALL_SHAPES[i].get()->getInfo();
        }
        else if (nullptr != dynamic_cast<Rectangle*>(ALL_SHAPES[i].get())) {
            cout << " ПРЯМОУГОЛЬНИК: " << ALL_SHAPES[i].get()->getInfo();
        }
        else if (nullptr != dynamic_cast<Triangle*>(ALL_SHAPES[i].get())) {
            cout << " ТРЕУГОЛЬНИК: " << ALL_SHAPES[i].get()->getInfo();
        }
        else if (nullptr != dynamic_cast<Polygon*>(ALL_SHAPES[i].get())) {
            cout << " МНОГОУГОЛЬНИК: " << ALL_SHAPES[i].get()->getInfo();
        }
        else {
            cout << "No info";
        }
        cout << '\n';
    }

}

void menu(int num)
{

    if (num == 1) {
        ALL_POINTS.clear();
        ALL_LINES.clear();
        ALL_SHAPES.clear();
        
    }
    else if (num == 2) {
        cout << "координаты точки (x, y): ";
        double x, y;
        cin >> x >> y;
        ALL_POINTS.push_back(Point(x, y));
        cout << '\n';

    }
    else if (num == 3) {
        cout << "Количество вершин: ";
        int c;
        cin >> c;
        double x, y;
        vector<Point> g;
        for (int i = 0; i < c; i++) {
            cout << "Вершина " << i + 1 << "-ая (x y): ";
            cin >> x >> y;
            g.push_back(Point(x, y));
        }
        ALL_SHAPES.push_back(make_unique<Polygon>(g));
        cout << '\n';

    }
    else if (num == 4) {
        double x, y, d;
        cout << "F1 (x, y): ";
        cin >> x >> y;
        Point F1(x, y);
        cout << "F2 (x, y): ";
        cin >> x >> y;
        Point F2(x, y);
        cout << "Большая полуось (длина): ";
        cin >> d;
        ALL_SHAPES.push_back(make_unique<Ellipse>(F1, F2, d));
        cout << '\n';

    }
    else if (num == 5)
    {
        cout << "Центр круга (x, y): ";
        double x, y, r;
        cin >> x >> y;
        cout << "Радиус (длина): ";
        cin >> r;
        ALL_SHAPES.push_back(make_unique<Circle>(Point(x, y), r));
        cout << '\n';

    }
    else if (num == 6) {
        double x, y, k;
        cout << "Вершина 1 (нижняя левая (х, у)): ";
        cin >> x >> y;
        Point P1(x, y);
        cout << "Вершина 2 (верхняя правая (х, у)): ";
        cin >> x >> y;
        Point P2(x, y);
        cout << "Отношение (введите 1 число): ";
        cin >> k;
        ALL_SHAPES.push_back(make_unique<Rectangle>(P1, P2, k));
        cout << '\n';
        
    }
    else if (num == 7) {
        double x, y;
        cout << "Вершина 1 (верхняя правая (х, у)): ";
        cin >> x >> y;
        Point P1(x, y);
        cout << "Вершина 2 (верхняя правая (х, у)): ";
        cin >> x >> y;
        Point P2(x, y);
        ALL_SHAPES.push_back(make_unique<Square>(P1, P2));
        cout << '\n';
        
    }
    else if (num == 8) {
        int c = 3;
        double x, y;
        vector<Point> g;
        for (int i = 0; i < c; i++) {
            cout << "Вершина " << i + 1 << "-ая (x,  y): ";
            cin >> x >> y;
            g.push_back(Point(x, y));
        }
        ALL_SHAPES.push_back(make_unique<Triangle>(g));
        cout << '\n';
        
    }
    else if (num == 9) {
        double x, y;
        cout << "Точка 1 (x, y): ";
        cin >> x >> y;
        Point P1(x, y);
        cout << "Точка 2 (x, y): ";
        cin >> x >> y;
        Point P2(x, y);
        ALL_LINES.push_back(Line(P1, P2));
        cout << '\n';
        
    }
    
    else if (num == 12) {
        int a = 1, x;
        cout << "Что можно изменить:  \n";
        print_all_shapes();
        cin >> x;
        if (x > ALL_POINTS.size() + ALL_LINES.size() + ALL_SHAPES.size()) {
            cout << "Введено неверно!\n";
        }
        else {
            if (x > ALL_POINTS.size() + ALL_LINES.size()) {
                x -= ALL_POINTS.size() + ALL_LINES.size() + 1;
                int p;
                print_edit_menu();
                cin >> p;
                switch (p)
                {
                case 1:
                    cout << "Центр (x, y): ";
                    double xx, y, deg;
                    cin >> xx >> y;
                    cout << "Степень: ";
                    cin >> deg;
                    ALL_SHAPES[x]->rotate(Point(xx, y), deg);
                    break;
                case 2:
                    cout << "Центр (x, y): ";
                    cin >> xx >> y;
                    ALL_SHAPES[x]->reflex(Point(xx, y));
                    break;
                case 3:
                    cout << "k(коэфф. наклона): "; cin >> xx;
                    cout << "b(сдвиг пооси х): "; cin  >> y;
                    ALL_SHAPES[x]->reflex(Line(xx, y));
                    break;
                case 4:
                    cout << "Центр (x, y): ";
                    cin >> xx >> y;
                    cout << "Коэффицент: ";
                    cin >> deg;
                    ALL_SHAPES[x]->scale(Point(xx, y), deg);
                    break;
                default:
                    cout << "Введено неверно!\n";
                    break;
                }
            }
            else {
                cout << "Нельзя измменить \n\n";
            }
        }
    }
    else if (num == 13) {
        int a = 1, x;
        cout << "Вы можете удалить: \n";
        print_all_shapes();
        cin >> x;
        if (x > ALL_POINTS.size() + ALL_LINES.size() + ALL_SHAPES.size()) {
            cout << "Введено неверно!\n";
        }
        else if (x >= ALL_POINTS.size() + ALL_LINES.size()) {
            x += -1 - ALL_POINTS.size() - ALL_LINES.size();
            vector< unique_ptr<Shape> > ::iterator pos = ALL_SHAPES.begin() + x;
            ALL_SHAPES.erase(pos);
        }
        else if (x >= ALL_POINTS.size()) {
            x += -1 - ALL_POINTS.size();
            vector<Line> ::iterator pos = ALL_LINES.begin() + x;
            ALL_LINES.erase(pos);
        }
        else {
            vector<Point> ::iterator pos = ALL_POINTS.begin() + x - 1;
            ALL_POINTS.erase(pos);
        }
        
    }
    else if (num == 14) {
        int a = 1, x;
        cout << "Периметр какой фигуры вы желаете получить?\n\n";
        print_all_shapes();
        cin >> x;
        if (x > ALL_POINTS.size() + ALL_LINES.size() + ALL_SHAPES.size()) {
            cout << "Введено неверно!\n";
        }
        else if (x > ALL_POINTS.size() + ALL_LINES.size()) {
            x -= 1 + ALL_POINTS.size() + ALL_LINES.size();
            cout << "P = " << ALL_SHAPES[x]->perimetr() << "\n\n";
        }
        else {
            cout << "P = " << 0 << "\n\n";
        }
    }
    else if (num == 15) {
        int a = 1, x;
        cout << "Площадь какой фигуры вы желаете получить? \n";
        print_all_shapes();
        cin >> x;
        if (x > ALL_POINTS.size() + ALL_LINES.size() + ALL_SHAPES.size()) {
            cout << "Введено неверно!\n\n";
        }
        else if (x > ALL_POINTS.size() + ALL_LINES.size()) {
            x -= 1 + ALL_POINTS.size() + ALL_LINES.size();
            cout << "S = " << ALL_SHAPES[x]->area() << "\n\n";
        }
        else {
            cout << "S = " << 0 << "\n\n";
        }
    }
    
    else if (num == 16) {
        int a = 1;
        cout << "Какие фигуры вы хотите проверить на равенство? \n";
        print_all_shapes();
        int i, j;
        cout << "Введите соответствующие номера: "; cout << '\n';
        cin >> i >> j;
        if (i != j) {
            if (i > ALL_POINTS.size() + ALL_LINES.size() + ALL_SHAPES.size() + ALL_SHAPES.size()
                || j > ALL_POINTS.size() + ALL_LINES.size() + ALL_SHAPES.size() + ALL_SHAPES.size()
                || i <= ALL_POINTS.size() + ALL_LINES.size() || j <= ALL_POINTS.size() + ALL_LINES.size()) {
                cout << "Wrong input!\n";
            }
            else {
                i -= 1 + ALL_POINTS.size() + ALL_LINES.size();
                j -= 1 + ALL_POINTS.size() + ALL_LINES.size();
                cout << (ALL_SHAPES[i].get() == ALL_SHAPES[j].get() ? "Равныы" : "Не равны") << '\n';
            }
        }
        else {
            cout << "!!! ОЩИБКА !!!\n";
        }
    }
    else if (num == 17) {
        int a = 1;
        cout << "Какие фигуры вы хотите сравнить? \n";
        print_all_shapes();
        int i, j;
        cout << "Введите соответствующие номера:"; cout << '\n';
        cin >> i >> j;
        if (i != j) {
            if (i > ALL_POINTS.size() + ALL_LINES.size() + ALL_SHAPES.size() + ALL_SHAPES.size()
                || j > ALL_POINTS.size() + ALL_LINES.size() + ALL_SHAPES.size() + ALL_SHAPES.size()
                || i <= ALL_POINTS.size() + ALL_LINES.size() || j <= ALL_POINTS.size() + ALL_LINES.size()) {
                cout << "Неверный ввод!\n";
            }
            else {
                i -= 1 + ALL_POINTS.size() + ALL_LINES.size();
                j -= 1 + ALL_POINTS.size() + ALL_LINES.size();
                cout << (ALL_SHAPES[i].get()->isCongruentTo(*ALL_SHAPES[j].get()) ? "Подобны" : "Не подобны") << '\n';
            }
        }
        else {
            cout << "!!! ОЩИБКА !!!\n";
        }
        }
    else if (num == 18) {
            int a = 1;
            cout << "У каких фигур вы хотите узнать подобие? \n";
            print_all_shapes();
            int i, j;
            cout << "Введите соответствующие номерф: "; cout << '\n';
            cin >> i >> j;
            if (i != j) {
                if (i > ALL_POINTS.size() + ALL_LINES.size() + ALL_SHAPES.size() + ALL_SHAPES.size()
                    || j > ALL_POINTS.size() + ALL_LINES.size() + ALL_SHAPES.size() + ALL_SHAPES.size()
                    || i <= ALL_POINTS.size() + ALL_LINES.size() || j <= ALL_POINTS.size() + ALL_LINES.size()) {
                    cout << "Неверный ввод!\n";
                }
                else {
                    i -= 1 + ALL_POINTS.size() + ALL_LINES.size();
                    j -= 1 + ALL_POINTS.size() + ALL_LINES.size();
                    cout << (ALL_SHAPES[i].get()->isSimilarTo(*ALL_SHAPES[j].get()) ? "ПОдобны" : "Не подобны") << '\n';
                }
            }
            else {
                cout << "!!! ОЩИБКА !!!\n";
            }
            }
    else if (num == 19) {
        int a = 1, x;
        cout << "Выбрать фигуру? \n";
        print_all_shapes();
        cin >> x;
        double i, j;
        cout << "Введите координаты точки (x, y): ";
        cin >> i >> j;
        Point P(i, j);
        if (x > ALL_POINTS.size() + ALL_LINES.size() + ALL_SHAPES.size()) {
            cout << "Введено неверно!\n";
        }
        else if (x > ALL_POINTS.size() + ALL_LINES.size()) {
            x -= 1 + ALL_POINTS.size() + ALL_LINES.size();
            cout << (ALL_SHAPES[x].get()->containsPoint(P) ? "True" : "False") << '\n';
        }
        else if (x > ALL_POINTS.size()) {
            x -= 1 + ALL_POINTS.size();
            cout << (P.y == ALL_LINES[x].k * P.x + ALL_LINES[x].b ? "True" : "False") << '\n';
        }
        else {
            cout << (ALL_POINTS[x - 1] == P ? "True" : "False") << '\n';
        }
    }
    glutPostRedisplay();
    return;
}


void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.5f, 0.5f, 0.5f);
    for (float x = -1.0f; x <= 1.0f; x += 0.025f) {
        for (float y = -1.0f; y <= 1.0f; y += 0.025f) {
            glBegin(GL_LINE_LOOP);
            glVertex2f(x - 0.1f, y - 0.1f);
            glVertex2f(x + 0.1f, y - 0.1f);
            glVertex2f(x + 0.1f, y + 0.1f);
            glVertex2f(x - 0.1f, y + 0.1f);
            glEnd();
        }
    }

    glBegin(GL_LINES);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f(0, -1);
    glVertex2f(0, 1);
    glVertex2f(-1, 0);
    glVertex2f(1, 0);
    glEnd();


    for (int i = 0; i < ALL_SHAPES.size(); i++) {
        ALL_SHAPES[i]->glutDraw();
    }

    glEnd();
    glFlush();
}


void createMenu(void)
{

    

    //фигуры
    submenu_id = glutCreateMenu(menu);
    glutAddMenuEntry("Точка", 2);
    glutAddSubMenu("Линия", 9);
    glutAddMenuEntry("Треугольник", 8);
    glutAddMenuEntry("Квадрат", 7);
    glutAddMenuEntry("Прямоугольник", 6);
    glutAddMenuEntry("Многоугольник", 3);
    glutAddMenuEntry("Эллипс", 4);
    glutAddMenuEntry("Круг", 5);


    //действия с фигурами
    submenu_info = glutCreateMenu(menu);
    glutAddMenuEntry("Периметр", 14);
    glutAddMenuEntry("Площадь", 15);
    glutAddMenuEntry("Равенство", 16);
    glutAddMenuEntry("Существование", 17);
    glutAddMenuEntry("Подобие", 18);
    glutAddMenuEntry("Содержит:", 19);

    //главное
    menu_id = glutCreateMenu(menu);
    glutAddSubMenu("Добавить", submenu_id);
    glutAddMenuEntry("Изменить", 12);
    glutAddSubMenu("Доп", submenu_info);
    glutAddMenuEntry("Удалить", 13);
    glutAddMenuEntry("Очистить", 1);

    glutAttachMenu(GLUT_RIGHT_BUTTON);
}


int main(int argc, char** argv) 
{
    setlocale(LC_ALL, "RU");
    

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
    glutInitWindowSize(1250, 900);
    glutInitWindowPosition(500, 10);
    window = glutCreateWindow("ФИГУРЫ");
    createMenu();
    glClearColor(1, 1, 1, 1.0);
    gluOrtho2D(-1.0, 1.0, -0.7, 0.7);
    glutDisplayFunc(display);
    glutMainLoop();
    return EXIT_SUCCESS;
}

