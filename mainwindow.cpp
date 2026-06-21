#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "FileHandler.h"
#include "Exceptions.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <vector>
using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , university("ENSIA")
    , loggedInStudent(nullptr)
{
    ui->setupUi(this);
    ui->dormitoriesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    try {
        FileHandler::loadData(university, ".");
    } catch (...) {}
}

MainWindow::~MainWindow()
{
    try {
        FileHandler::saveData(university, ".");
    } catch (...) {}
    delete ui;
}

void MainWindow::on_btnLogin_clicked()
{
    QString username = ui->usernameEdit->text();
    QString password = ui->passwordEdit->text();

    if(username == "admin" && password == "1234"){
        ui->lblYearStatic->setCurrentWidget(ui->dashboardPage);
        updateStats();
        return;
    }

    vector<Dormitory>& dorms = university.getDormitories();
    for(size_t i = 0; i < dorms.size(); i++){
        vector<Room>& rooms = dorms[i].getRooms();
        for(size_t j = 0; j < rooms.size(); j++){
            const vector<Student*>& students = rooms[j].getStudents();
            for(size_t k = 0; k < students.size(); k++){
                if(students[k]->getName() == username.toStdString() &&
                    students[k]->getId() == password.toStdString()){
                    loggedInStudent = students[k];
                    populateStudentView();
                    ui->lblYearStatic->setCurrentWidget(ui->studentViewPage);
                    return;
                }
            }
        }
    }

    QMessageBox::warning(this, "Error", "Wrong username or password!");
    ui->usernameEdit->clear();
    ui->passwordEdit->clear();
    ui->usernameEdit->setFocus();
}

void MainWindow::on_btnAddDormitory_clicked()
{
    bool ok;
    QString name = QInputDialog::getText(this, "Add Dormitory", "Enter dormitory name:", QLineEdit::Normal, "", &ok);
    if(!ok || name.isEmpty()) return;

    int capacity = QInputDialog::getInt(this, "Add Dormitory", "Enter capacity:", 100, 1, 1000, 1, &ok);
    if(!ok) return;

    int numRooms = QInputDialog::getInt(this, "Add Dormitory", "Enter number of rooms:", 10, 1, 100, 1, &ok);
    if(!ok) return;

    int roomCapacity = QInputDialog::getInt(this, "Add Dormitory", "Enter capacity per room:", 2, 1, 10, 1, &ok);
    if(!ok) return;

    Dormitory dorm(name.toStdString(), capacity);
    for(int i = 1; i <= numRooms; i++){
        dorm.addRoom(Room(i, roomCapacity));
    }
    university.addDormitory(dorm);
    QMessageBox::information(this, "Success", "Dormitory added successfully!");
    loadDormitoriesTable();
    updateStats();
}

void MainWindow::on_btnAddStudent_clicked()
{
    bool ok;
    vector<Dormitory>& dorms = university.getDormitories();
    if(dorms.empty()){
        QMessageBox::warning(this, "Error", "No dormitories available! Please add a dormitory first.");
        return;
    }

    QString name = QInputDialog::getText(this, "Add Student", "Enter name:", QLineEdit::Normal, "", &ok);
    if(!ok || name.isEmpty()) return;
    QString id = QInputDialog::getText(this, "Add Student", "Enter ID:", QLineEdit::Normal, "", &ok);
    if(!ok || id.isEmpty()) return;
    int year = QInputDialog::getInt(this, "Add Student", "Enter year:", 1, 1, 5, 1, &ok);
    if(!ok) return;

    // Build dormitory list for selection
    QStringList dormNames;
    for(size_t i = 0; i < dorms.size(); i++){
        dormNames << QString::fromStdString(dorms[i].getName());
    }
    QString selectedDorm = QInputDialog::getItem(this, "Add Student", "Select dormitory:", dormNames, 0, false, &ok);
    if(!ok) return;

    try {
        Student* s = new Student(name.toStdString(), id.toStdString(), year);
        bool added = false;
        for(size_t i = 0; i < dorms.size(); i++){
            if(dorms[i].getName() == selectedDorm.toStdString()){
                if(dorms[i].addStudent(s)){
                    added = true;
                } else {
                    QMessageBox::warning(this, "Error", "All rooms in this dormitory are full!");
                    delete s;
                }
                break;
            }
        }
        if(added){
            QMessageBox::information(this, "Success", "Student added successfully!");
            loadStudentsTable();
            updateStats();
        }
    } catch(InvalidYearException& e){
        QMessageBox::warning(this, "Error", e.what());
    } catch(InvalidInputException& e){
        QMessageBox::warning(this, "Error", e.what());
    } catch(exception& e){
        QMessageBox::warning(this, "Error", e.what());
    }
}

void MainWindow::on_btnRemoveStudent_clicked()
{
    bool ok;
    QString id = QInputDialog::getText(this, "Remove Student", "Enter student ID:", QLineEdit::Normal, "", &ok);
    if(!ok || id.isEmpty()) return;

    vector<Dormitory>& dorms = university.getDormitories();
    for(size_t i = 0; i < dorms.size(); i++){
        if(dorms[i].removeStudent(id.toStdString())){
            QMessageBox::information(this, "Success", "Student removed successfully!");
            loadStudentsTable();
            updateStats();
            return;
        }
    }
    QMessageBox::warning(this, "Error", "Student not found!");
}

void MainWindow::on_btnDisplayDormitories_clicked()
{
    QString result = "";
    vector<Dormitory>& dorms = university.getDormitories();
    if(dorms.empty()){
        QMessageBox::information(this, "Dormitories", "No dormitories found!");
        return;
    }
    for(size_t i = 0; i < dorms.size(); i++){
        result += "Dormitory: " + QString::fromStdString(dorms[i].getName()) + "\n";
        vector<Room>& rooms = dorms[i].getRooms();
        for(size_t j = 0; j < rooms.size(); j++){
            result += "  Room " + QString::number(rooms[j].getRoomNumber()) + ":\n";
            const vector<Student*>& students = rooms[j].getStudents();
            for(size_t k = 0; k < students.size(); k++){
                result += "    - " + QString::fromStdString(students[k]->getName()) +
                          " (ID: " + QString::fromStdString(students[k]->getId()) + ")\n";
            }
        }
    }
    QMessageBox::information(this, "Dormitories", result);
}

void MainWindow::on_btnManageRestaurant_clicked()
{
    bool ok;
    vector<Dormitory>& dorms = university.getDormitories();
    if(dorms.empty()){
        QMessageBox::warning(this, "Error", "No dormitories available!");
        return;
    }

    QString breakfast = QInputDialog::getText(this, "Restaurant", "Enter breakfast:", QLineEdit::Normal, "", &ok);
    if(!ok) return;
    QString lunch = QInputDialog::getText(this, "Restaurant", "Enter lunch:", QLineEdit::Normal, "", &ok);
    if(!ok) return;
    QString dinner = QInputDialog::getText(this, "Restaurant", "Enter dinner:", QLineEdit::Normal, "", &ok);
    if(!ok) return;

    dorms[0].getRestaurant().setBreakfast(breakfast.toStdString());
    dorms[0].getRestaurant().setLunch(lunch.toStdString());
    dorms[0].getRestaurant().setDinner(dinner.toStdString());

    QMessageBox::information(this, "Success", "Menu updated successfully!");
}

// Dashboard sidebar
void MainWindow::on_btnHome_clicked(){
    ui->lblYearStatic->setCurrentWidget(ui->dashboardPage);
    updateStats();
}
void MainWindow::on_btnStudents_clicked(){
    ui->lblYearStatic->setCurrentWidget(ui->studentsPage);
    loadStudentsTable();
}
void MainWindow::on_btnDormitories_clicked(){
    ui->lblYearStatic->setCurrentWidget(ui->dormitoriesPage);
    loadDormitoriesTable();
}
void MainWindow::on_btnRestaurant_clicked(){
    ui->lblYearStatic->setCurrentWidget(ui->restaurentPage);
    loadRestaurantPage();
}

// Students page sidebar
void MainWindow::on_btnHome2_clicked(){
    ui->lblYearStatic->setCurrentWidget(ui->dashboardPage);
    updateStats();
}
void MainWindow::on_btnStudents2_clicked(){
    ui->lblYearStatic->setCurrentWidget(ui->studentsPage);
    loadStudentsTable();
}
void MainWindow::on_btnDormitories2_clicked(){
    ui->lblYearStatic->setCurrentWidget(ui->dormitoriesPage);
    loadDormitoriesTable();
}
void MainWindow::on_btnRestaurant2_clicked(){
    ui->lblYearStatic->setCurrentWidget(ui->restaurentPage);
    loadRestaurantPage();
}

// Students page buttons
void MainWindow::on_btnAddStudent2_clicked(){
    on_btnAddStudent_clicked();
}
void MainWindow::on_btnRemoveStudent2_clicked(){
    on_btnRemoveStudent_clicked();
}

// Dormitories page sidebar
void MainWindow::on_btnHome3_clicked(){
    ui->lblYearStatic->setCurrentWidget(ui->dashboardPage);
    updateStats();
}
void MainWindow::on_btnStudents3_clicked(){
    ui->lblYearStatic->setCurrentWidget(ui->studentsPage);
    loadStudentsTable();
}
void MainWindow::on_btnDormitories3_clicked(){
    ui->lblYearStatic->setCurrentWidget(ui->dormitoriesPage);
    loadDormitoriesTable();
}
void MainWindow::on_btnRestaurant3_clicked(){
    ui->lblYearStatic->setCurrentWidget(ui->restaurentPage);
    loadRestaurantPage();
}

// Dormitories page buttons
void MainWindow::on_btnAddDormitory2_clicked(){
    on_btnAddDormitory_clicked();
}
void MainWindow::on_btnRemoveDormitory2_clicked(){
    on_btnRemoveDormitory_clicked();
}

// Restaurant page sidebar
void MainWindow::on_btnHome4_clicked(){
    ui->lblYearStatic->setCurrentWidget(ui->dashboardPage);
    updateStats();
}
void MainWindow::on_btnStudents4_clicked(){
    ui->lblYearStatic->setCurrentWidget(ui->studentsPage);
    loadStudentsTable();
}
void MainWindow::on_btnDormitories4_clicked(){
    ui->lblYearStatic->setCurrentWidget(ui->dormitoriesPage);
    loadDormitoriesTable();
}
void MainWindow::on_btnRestaurant4_clicked(){
    ui->lblYearStatic->setCurrentWidget(ui->restaurentPage);
    loadRestaurantPage();
}

void MainWindow::on_btnUpdateMenu_clicked()
{
    bool ok;
    vector<Dormitory>& dorms = university.getDormitories();
    if(dorms.empty()){
        QMessageBox::warning(this, "Error", "No dormitories available!");
        return;
    }

    int index = ui->dormitorySelector->currentIndex();
    if(index < 0 || index >= (int)dorms.size()) return;

    QString breakfast = QInputDialog::getText(this, "Restaurant", "Enter breakfast:", QLineEdit::Normal, "", &ok);
    if(!ok) return;
    QString lunch = QInputDialog::getText(this, "Restaurant", "Enter lunch:", QLineEdit::Normal, "", &ok);
    if(!ok) return;
    QString dinner = QInputDialog::getText(this, "Restaurant", "Enter dinner:", QLineEdit::Normal, "", &ok);
    if(!ok) return;

    dorms[index].getRestaurant().setBreakfast(breakfast.toStdString());
    dorms[index].getRestaurant().setLunch(lunch.toStdString());
    dorms[index].getRestaurant().setDinner(dinner.toStdString());

    QMessageBox::information(this, "Success", "Menu updated successfully!");
    loadRestaurantPage();
}

void MainWindow::on_dormitorySelector_currentIndexChanged(int index)
{
    vector<Dormitory>& dorms = university.getDormitories();
    if(index < 0 || index >= (int)dorms.size()) return;

    ui->lblBreakfast->setText(QString::fromStdString(dorms[index].getRestaurant().getBreakfast()));
    ui->lblLunch->setText(QString::fromStdString(dorms[index].getRestaurant().getLunch()));
    ui->lblDinner->setText(QString::fromStdString(dorms[index].getRestaurant().getDinner()));
}

void MainWindow::on_btnRemoveDormitory_clicked()
{
    bool ok;
    QString name = QInputDialog::getText(this, "Remove Dormitory", "Enter dormitory name:", QLineEdit::Normal, "", &ok);
    if(!ok || name.isEmpty()) return;

    university.removeDormitory(name.toStdString());
    QMessageBox::information(this, "Success", "If the dormitory existed, it has been removed.");
    loadDormitoriesTable();
    updateStats();
}

void MainWindow::updateStats()
{
    int totalStudents = 0;
    int totalRooms = 0;
    vector<Dormitory>& dorms = university.getDormitories();

    for(size_t i = 0; i < dorms.size(); i++){
        vector<Room>& rooms = dorms[i].getRooms();
        totalRooms += rooms.size();
        for(size_t j = 0; j < rooms.size(); j++){
            totalStudents += rooms[j].getStudents().size();
        }
    }

    ui->lblTotalStudents->setText(QString::number(totalStudents));
    ui->lblTotalDormitories->setText(QString::number(dorms.size()));
    ui->lblTotalRooms->setText(QString::number(totalRooms));
}

void MainWindow::loadStudentsTable()
{
    ui->studentsTable->setRowCount(0);
    ui->studentsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    vector<Dormitory>& dorms = university.getDormitories();
    for(size_t i = 0; i < dorms.size(); i++){
        vector<Room>& rooms = dorms[i].getRooms();
        for(size_t j = 0; j < rooms.size(); j++){
            const vector<Student*>& students = rooms[j].getStudents();
            for(size_t k = 0; k < students.size(); k++){
                int row = ui->studentsTable->rowCount();
                ui->studentsTable->insertRow(row);
                ui->studentsTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(students[k]->getName())));
                ui->studentsTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(students[k]->getId())));
                ui->studentsTable->setItem(row, 2, new QTableWidgetItem(QString::number(students[k]->getYear())));
                ui->studentsTable->setItem(row, 3, new QTableWidgetItem(QString::number(rooms[j].getRoomNumber())));
                ui->studentsTable->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(dorms[i].getName())));
            }
        }
    }
}

void MainWindow::loadDormitoriesTable()
{
    ui->dormitoriesTable->setRowCount(0);
    ui->dormitoriesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    vector<Dormitory>& dorms = university.getDormitories();
    for(size_t i = 0; i < dorms.size(); i++){
        int totalStudents = 0;
        vector<Room>& rooms = dorms[i].getRooms();
        for(size_t j = 0; j < rooms.size(); j++){
            totalStudents += rooms[j].getStudents().size();
        }
        int row = ui->dormitoriesTable->rowCount();
        ui->dormitoriesTable->insertRow(row);
        ui->dormitoriesTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(dorms[i].getName())));
        ui->dormitoriesTable->setItem(row, 1, new QTableWidgetItem(QString::number(dorms[i].getCapacity())));
        ui->dormitoriesTable->setItem(row, 2, new QTableWidgetItem(QString::number(rooms.size())));
        ui->dormitoriesTable->setItem(row, 3, new QTableWidgetItem(QString::number(totalStudents)));
    }
}

void MainWindow::loadRestaurantPage()
{
    ui->dormitorySelector->clear();
    vector<Dormitory>& dorms = university.getDormitories();
    if(dorms.empty()) return;

    for(size_t i = 0; i < dorms.size(); i++){
        ui->dormitorySelector->addItem(QString::fromStdString(dorms[i].getName()));
    }

    ui->lblBreakfast->setText(QString::fromStdString(dorms[0].getRestaurant().getBreakfast()));
    ui->lblLunch->setText(QString::fromStdString(dorms[0].getRestaurant().getLunch()));
    ui->lblDinner->setText(QString::fromStdString(dorms[0].getRestaurant().getDinner()));
}

void MainWindow::populateStudentView()
{
    if(loggedInStudent == nullptr) return;

    QString welcomeText = "Welcome, " + QString::fromStdString(loggedInStudent->getName()) + "!";
    ui->lblStudentWelcome->setText(welcomeText);

    ui->lblStudentName->setText(QString::fromStdString(loggedInStudent->getName()));
    ui->lblStudentId->setText(QString::fromStdString(loggedInStudent->getId()));
    ui->lblStudentYear->setText(QString::number(loggedInStudent->getYear()));

    // Find which dormitory and room this student belongs to
    vector<Dormitory>& dorms = university.getDormitories();
    bool found = false;
    for(size_t i = 0; i < dorms.size() && !found; i++){
        vector<Room>& rooms = dorms[i].getRooms();
        for(size_t j = 0; j < rooms.size() && !found; j++){
            const vector<Student*>& students = rooms[j].getStudents();
            for(size_t k = 0; k < students.size(); k++){
                if(students[k]->getId() == loggedInStudent->getId()){
                    ui->lblStudentDormitory->setText(QString::fromStdString(dorms[i].getName()));
                    ui->lblStudentRoom->setText(QString::number(rooms[j].getRoomNumber()));
                    found = true;
                    break;
                }
            }
        }
    }

    if(!found){
        ui->lblStudentDormitory->setText("Not assigned");
        ui->lblStudentRoom->setText("Not assigned");
    }
}

void MainWindow::clearStudentView(){
    ui->lblStudentWelcome->setText("Welcome :");
    ui->lblStudentName->setText("");
    ui->lblStudentId->setText("");
    ui->lblStudentYear->setText("");
    ui->lblStudentDormitory->setText("");
    ui->lblStudentRoom->setText("");
}

void MainWindow::on_btnLogoutStudent_clicked(){
    loggedInStudent = nullptr;
    ui->usernameEdit->clear();
    ui->passwordEdit->clear();
    clearStudentView();
    ui->lblYearStatic->setCurrentWidget(ui->loginPage);
}

void MainWindow::on_btnLogout_clicked(){
    loggedInStudent = nullptr;
    ui->usernameEdit->clear();
    ui->passwordEdit->clear();
    clearStudentView();
    ui->lblYearStatic->setCurrentWidget(ui->loginPage);
}
void MainWindow::on_btnLogout2_clicked(){
    loggedInStudent = nullptr;
    ui->usernameEdit->clear();
    ui->passwordEdit->clear();
    clearStudentView();
    ui->lblYearStatic->setCurrentWidget(ui->loginPage);
}
void MainWindow::on_btnLogout3_clicked(){
    loggedInStudent = nullptr;
    ui->usernameEdit->clear();
    ui->passwordEdit->clear();
    clearStudentView();
    ui->lblYearStatic->setCurrentWidget(ui->loginPage);
}
void MainWindow::on_btnLogout4_clicked(){
    loggedInStudent = nullptr;
    ui->usernameEdit->clear();
    ui->passwordEdit->clear();
    clearStudentView();
    ui->lblYearStatic->setCurrentWidget(ui->loginPage);
}

// ===== Student view <-> Student menu navigation =====

void MainWindow::on_btnViewMenu_clicked()
{
    if(loggedInStudent == nullptr) return;

    vector<Dormitory>& dorms = university.getDormitories();
    for(size_t i = 0; i < dorms.size(); i++){
        vector<Room>& rooms = dorms[i].getRooms();
        for(size_t j = 0; j < rooms.size(); j++){
            const vector<Student*>& students = rooms[j].getStudents();
            for(size_t k = 0; k < students.size(); k++){
                if(students[k]->getId() == loggedInStudent->getId()){
                    Restaurant& r = dorms[i].getRestaurant();
                    ui->lblMenuBreakfast1->setText(QString::fromStdString(r.getBreakfast()));
                    ui->lblMenuLunch1->setText(QString::fromStdString(r.getLunch()));
                    ui->lblMenuDinner1->setText(QString::fromStdString(r.getDinner()));
                    ui->lblYearStatic->setCurrentWidget(ui->studentMenuPage);
                    return;
                }
            }
        }
    }

    // Student isn't assigned to a room/dormitory yet
    ui->lblMenuBreakfast1->setText("Not available");
    ui->lblMenuLunch1->setText("Not available");
    ui->lblMenuDinner1->setText("Not available");
    ui->lblYearStatic->setCurrentWidget(ui->studentMenuPage);
}

void MainWindow::on_btnViewStudent_clicked()
{
    ui->lblYearStatic->setCurrentWidget(ui->studentViewPage);
}

void MainWindow::on_btnLogoutStudentMenu_clicked()
{
    loggedInStudent = nullptr;
    ui->usernameEdit->clear();
    ui->passwordEdit->clear();
    clearStudentView();
    ui->lblYearStatic->setCurrentWidget(ui->loginPage);
}

// ===== Rooms page (admin) =====

void MainWindow::on_btnViewRooms_clicked()
{
    int row = ui->dormitoriesTable->currentRow();
    if(row < 0) {
        QMessageBox::warning(this, "Error", "Please select a dormitory first!");
        return;
    }

    QString dormName = ui->dormitoriesTable->item(row, 0)->text();
    vector<Dormitory>& dorms = university.getDormitories();

    for(size_t i = 0; i < dorms.size(); i++){
        if(dorms[i].getName() == dormName.toStdString()){
            ui->lblRoomsTitle->setText("Rooms in " + dormName + ":");

            vector<Room>& rooms = dorms[i].getRooms();
            ui->roomsTable->setRowCount(0);
            ui->roomsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

            for(size_t j = 0; j < rooms.size(); j++){
                int occupied = rooms[j].getStudents().size();
                int r = ui->roomsTable->rowCount();
                ui->roomsTable->insertRow(r);
                ui->roomsTable->setItem(r, 0, new QTableWidgetItem(QString::number(rooms[j].getRoomNumber())));
                ui->roomsTable->setItem(r, 1, new QTableWidgetItem(QString::number(rooms[j].getCapacity())));
                ui->roomsTable->setItem(r, 2, new QTableWidgetItem(QString::number(occupied)));
                ui->roomsTable->setItem(r, 3, new QTableWidgetItem(rooms[j].isFull() ? "Full" : "Available"));
            }

            ui->lblYearStatic->setCurrentWidget(ui->roomsPage);
            return;
        }
    }
}

void MainWindow::on_btnHome5_clicked(){
    ui->lblYearStatic->setCurrentWidget(ui->dashboardPage);
    updateStats();
}
void MainWindow::on_btnStudents5_clicked(){
    ui->lblYearStatic->setCurrentWidget(ui->studentsPage);
    loadStudentsTable();
}
void MainWindow::on_btnDormitories5_clicked(){
    ui->lblYearStatic->setCurrentWidget(ui->dormitoriesPage);
    loadDormitoriesTable();
}
void MainWindow::on_btnRestaurant5_clicked(){
    ui->lblYearStatic->setCurrentWidget(ui->restaurentPage);
    loadRestaurantPage();
}
void MainWindow::on_btnLogout5_clicked(){
    loggedInStudent = nullptr;
    ui->usernameEdit->clear();
    ui->passwordEdit->clear();
    clearStudentView();
    ui->lblYearStatic->setCurrentWidget(ui->loginPage);
}