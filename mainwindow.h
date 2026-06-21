#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "University.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void on_btnLogin_clicked();
    void on_btnAddDormitory_clicked();
    void on_btnAddStudent_clicked();
    void on_btnRemoveStudent_clicked();
    void on_btnDisplayDormitories_clicked();
    void on_btnManageRestaurant_clicked();
    void on_btnHome_clicked();
    void on_btnStudents_clicked();
    void on_btnDormitories_clicked();
    void on_btnRestaurant_clicked();
    void on_btnLogout_clicked();
    void on_btnHome2_clicked();
    void on_btnStudents2_clicked();
    void on_btnDormitories2_clicked();
    void on_btnRestaurant2_clicked();
    void on_btnLogout2_clicked();
    void on_btnAddStudent2_clicked();
    void on_btnRemoveStudent2_clicked();
    void on_btnHome3_clicked();
    void on_btnStudents3_clicked();
    void on_btnDormitories3_clicked();
    void on_btnRestaurant3_clicked();
    void on_btnLogout3_clicked();
    void on_btnAddDormitory2_clicked();
    void on_btnRemoveDormitory2_clicked();
    void on_btnHome4_clicked();
    void on_btnStudents4_clicked();
    void on_btnDormitories4_clicked();
    void on_btnRestaurant4_clicked();
    void on_btnLogout4_clicked();
    void on_btnUpdateMenu_clicked();
    void on_btnRemoveDormitory_clicked();
    void on_dormitorySelector_currentIndexChanged(int index);
    void on_btnLogoutStudent_clicked();

    // Student view <-> Student menu navigation
    void on_btnViewMenu_clicked();
    void on_btnViewStudent_clicked();
    void on_btnLogoutStudentMenu_clicked();

    // Rooms page (admin)
    void on_btnViewRooms_clicked();
    void on_btnHome5_clicked();
    void on_btnStudents5_clicked();
    void on_btnDormitories5_clicked();
    void on_btnRestaurant5_clicked();
    void on_btnLogout5_clicked();

private:
    Ui::MainWindow *ui;
    University university;
    Student* loggedInStudent;
    void updateStats();
    void loadStudentsTable();
    void loadDormitoriesTable();
    void loadRestaurantPage();
    void populateStudentView();
    void clearStudentView();
};
#endif