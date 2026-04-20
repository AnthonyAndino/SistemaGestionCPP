#include "mainwindow.h"

// Librerías del proyecto (gestión de datos, logs y UI)
#include "FileHandler.h"
#include "SocioManager.h"
#include "LogManager.h"
#include "LogViewer.h"

#include "./ui_mainwindow.h"

// Librerías estándar y Qt
#include <fstream>
#include <QMessageBox>
#include <vector>
#include <QDesktopServices>
#include <QUrl>
#include <QWidget>
#include <Qt>
#include <QIcon>


/**
 * @brief Constructor principal de la ventana
 * Configura UI, estilos, tabla y carga datos iniciales
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Iconos en los inputs (usuario, dinero, búsqueda)
    QAction *iconNombre = ui->txtNombre->addAction(QIcon(":/icons/icons/user.png"), QLineEdit::LeadingPosition);
    QAction *iconSaldo = ui->txtSaldo->addAction(QIcon(":/icons/icons/money.png"), QLineEdit::LeadingPosition);
    QAction *iconBuscar = ui->txtBuscar->addAction(QIcon(":/icons/icons/search.png"), QLineEdit::LeadingPosition);

    // Padding para evitar choque con iconos
    ui->txtNombre->setStyleSheet("padding-left: 35px;");
    ui->txtSaldo->setStyleSheet("padding-left: 35px;");
    ui->txtBuscar->setStyleSheet("padding-left: 35px;");

    // Tema oscuro global de la aplicación
    this->setStyleSheet(R"(
        QMainWindow {
            background-color: #121212;
        }

        QLabel {
            color: #ffffff;
            font-size: 14px;
        }

        QLineEdit {
            background-color: #1e1e1e;
            color: #ffffff;
            border: 1px solid #333;
            border-radius: 5px;
            padding: 6px 6px 6px 30px; /* IMPORTANTE: El 30px es para que el texto no tape el icono */
        }

        QLineEdit:focus {
            border: 1px solid #2d89ef;
        }

        QPushButton {
            background-color: #2d89ef;
            color: white;
            border-radius: 5px;
            padding: 8px 15px;
            font-weight: bold;
        }

        QPushButton:hover { background-color: #1b5fad; }
        QPushButton:pressed { background-color: #144a8a; }

        QTableWidget {
            background-color: #1e1e1e;
            color: #ffffff;
            gridline-color: #333;
            border-radius: 8px;
            selection-background-color: #2d89ef;
        }

        QHeaderView::section {
            background-color: #252525;
            color: #aaaaaa;
            padding: 10px;
            border: none;
            font-weight: bold;
            text-transform: uppercase;
            font-size: 11px;
        }

        QFrame {
            background-color: #1e1e1e;
            border-radius: 10px;
            border: 1px solid #2a2a2a;
        }

        /* Estilo específico para los textos grandes del Dashboard */
        #lblTotalSociosCard, #lblSaldoTotalCard, #lblPromedioCard {
            font-size: 20px;
            font-weight: bold;
            color: #2d89ef;
            border: none; /* Quita el borde si el label está dentro del frame */
        }
    )");

    // Configuración de la tabla de socios
    ui->tableSocios->horizontalHeader()->setStretchLastSection(true);
    ui->tableSocios->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableSocios->setAlternatingRowColors(true);
    ui->tableSocios->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableSocios->setShowGrid(false);
    ui->tableSocios->verticalHeader()->setVisible(false);
    ui->tableSocios->setColumnCount(3);
    ui->tableSocios->setHorizontalHeaderLabels({"ID", "Nombre", "Saldo"});
    ui->tableSocios->setSortingEnabled(true);
    ui->tableSocios->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableSocios->setColumnWidth(0, 80);
    ui->tableSocios->horizontalHeader()->setStretchLastSection(true);
    ui->tableSocios->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->btnGuardar->setStyleSheet("background-color: #28a745; color: white; font-weight: bold;");
    ui->btnEliminar->setStyleSheet("background-color: #dc3545; color: white; font-weight: bold;");

    // Carga inicial de datos
    cargarDatos();

    // Actualiza métricas
    actualizarDashboard();
}

MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief Guarda un nuevo socio en archivo
 */
void MainWindow::on_btnGuardar_clicked()
{

    QString nombre = ui->txtNombre->text().trimmed();
    QString saldoStr = ui->txtSaldo->text().trimmed();

    bool ok;
    double saldo = saldoStr.toDouble(&ok);

    if(nombre.isEmpty()){
        QMessageBox::warning(this, "Error", "Ingrese un nombre");
        return;
    }

    if(!ok){
        QMessageBox::warning(this, "Error", "Saldo invalio");
        return;
    }

    auto datos = FileHandler::leerArchivo("socios.txt");

    SocioManager::agregarSocio(datos, nombre.toStdString(), saldo);

    FileHandler::escribirArchivo("socios.txt", datos);

    LogManager::registrar("AGREGADO: " + nombre.toStdString() + " - L. " + std::to_string(saldo));

    QMessageBox::information(this, "OK", "Guardado");

    cargarDatos();
    actualizarDashboard();

    ui->txtNombre->clear();
    ui->txtSaldo->clear();

}

/**
 * @brief Lee archivo y muestra socios en la tabla
 */
void MainWindow::cargarDatos(){

    ui->tableSocios->setRowCount(0);

    std::vector<std::string> datos = FileHandler::leerArchivo("socios.txt");

    int fila = 0;

    for(const auto& linea : datos){

        size_t pos1 = linea.find(",");
        size_t pos2 = linea.find(",", pos1 + 1);

        if(pos1 == std::string::npos || pos2 == std::string::npos) continue;

        std::string id = linea.substr(0, pos1);
        std::string nombre = linea.substr(pos1 + 1, pos2 - pos1 - 1);
        std::string saldoStr = linea.substr(pos2 + 1);

        double saldo = 0;

        try{
            saldo = std::stod(saldoStr);
        }catch(...){

        }

        ui->tableSocios->insertRow(fila);

        QTableWidgetItem *itemID = new QTableWidgetItem(QString::fromStdString(id));
        itemID->setFlags(itemID->flags() & ~Qt::ItemIsEditable);

        ui->tableSocios->setItem(fila, 0, itemID);
        ui->tableSocios->setItem(fila, 1, new QTableWidgetItem(QString::fromStdString(nombre)));

        QString saldoFormateado = "L. " + QLocale().toString(saldo, 'f', 2);

        QTableWidgetItem *itemSaldo = new QTableWidgetItem(saldoFormateado);
        itemSaldo->setData(Qt::UserRole, saldo);
        itemSaldo->setToolTip("Saldo real: " + QString::number(saldo));

        if(saldo > 0){
            itemSaldo->setForeground(QBrush(Qt::green));
        }
        else if(saldo < 0){
            itemSaldo->setForeground(QBrush(Qt::red));
        }
        else{
            itemSaldo->setForeground(QBrush(Qt::white));
        }

        ui->tableSocios->setItem(fila, 2, itemSaldo);

        fila++;

    }

}

/**
 * @brief Elimina el socio seleccionado de la tabla y archivo
 */
void MainWindow::on_btnEliminar_clicked()
{

    int fila = ui->tableSocios->currentRow();

    if(fila < 0){
        QMessageBox::warning(this, "Error", "Seleccione un registro");
        return;
    }

    QString id = ui->tableSocios->item(fila, 0)->text();

    auto reply = QMessageBox::question(this, "Confirmar", "Eliminar registro?", QMessageBox::Yes | QMessageBox::No);

    if(reply == QMessageBox::No) return;

    auto datos = FileHandler::leerArchivo("socios.txt");

    SocioManager::eliminarSocio(datos, id.toStdString());

    FileHandler::escribirArchivo("socios.txt", datos);

    LogManager::registrar("ELIMINADO: ID " + id.toStdString());

    cargarDatos();
    actualizarDashboard();

    QMessageBox::information(this, "OK", "Eliminado");

}

/**
 * @brief Filtra socios según texto ingresado
 */
void MainWindow::on_txtBuscar_textChanged(const QString &text){

    cargandoDatos = true;
    //Funcion que se ejecuta cada vez que el usuario escribe algo

    //Convertir el texto de busqueadd a string normal
    std::string filtro = text.toStdString();

    //Abrimos el archivo
    std::ifstream archivo("socios.txt");
    std::string linea;

    //Limpiar la tabla antes de volver a llenarla
    ui->tableSocios->setRowCount(0);

    int fila = 0;

    //Rrecorremos el archivo linea por linea
    while(getline(archivo, linea)){
        //verificamos si la linea contiene el texto buscado
        if(filtro.empty() || linea.find(filtro) != std::string::npos){

            size_t pos1 = linea.find(",");
            size_t pos2 = linea.find(",", pos1 + 1);

            if(pos1 != std::string::npos && pos2 != std::string::npos){

                std::string id = linea.substr(0, pos1);
                std::string nombre = linea.substr(pos1 + 1, pos2 - pos1 - 1);
                std::string saldo = linea.substr(pos2 + 1);

                //Si coincide se muestra en la tabla
                ui->tableSocios->insertRow(fila);
                ui->tableSocios->setItem(fila, 0, new QTableWidgetItem(QString::fromStdString(id)));
                ui->tableSocios->setItem(fila, 1, new QTableWidgetItem(QString::fromStdString(nombre)));
                ui->tableSocios->setItem(fila, 2, new QTableWidgetItem(QString::fromStdString(saldo)));
                fila++;
            }

        }
    }

    archivo.close();

    cargandoDatos = false;
}

/**
 * @brief Exporta la tabla a archivo CSV
 */
void MainWindow::on_btnExportar_clicked(){
    //abrir archivo csv
    std::ofstream archivo("socios_export.csv");

    //validar que abra el archivo
    if(!archivo.is_open()){
        QMessageBox::critical(this, "Error", "No se pudo crear el archivo");
        return;
    }

    archivo << "\xEF\xBB\xBF";

    //encabezado
    archivo << "ID,Nombre,Saldo" <<std::endl;

    //Rrecorrer las filas de las tablas
    for(int i = 0; i < ui->tableSocios->rowCount(); i++){
        //obtener datos de cada columna
        QString id = ui->tableSocios->item(i, 0)->text();
        QString nombre = ui->tableSocios->item(i, 1)->text();

        //obtener saldo real
        double saldo = ui->tableSocios->item(i, 2)->data(Qt::UserRole).toDouble();

        //formato CSV
        archivo << id.toStdString() << "," << "\"" << nombre.toStdString() << "\"" << "," << saldo << std::endl;
    }

    archivo.close();

    QMessageBox::information(this, "OK", "Exportado a Excel correctamente");

    QDesktopServices::openUrl(QUrl::fromLocalFile("socios_export.csv"));

}

/**
 * @brief Calcula total de socios, saldo total y promedio
 */
void MainWindow::actualizarDashboard(){
    //contadores
    int totalSocios = 0;
    double sumaSaldo = 0;

    //leer archivo
    std::ifstream archivo("socios.txt");
    std::string linea;

    while(getline(archivo, linea)){

        size_t pos1 = linea.find(",");
        size_t pos2 = linea.find(",", pos1 + 1);

        if(pos1 == std::string::npos || pos2 == std::string::npos) continue;

        std::string saldoStr = linea.substr(pos2 + 1);

        try{
            if(!saldoStr.empty()){
                double saldo = std::stod(saldoStr);
                sumaSaldo += saldo;
                totalSocios++;
            }
        } catch (...){
            continue;
        }
    }


    archivo.close();

    //calculo promedio
    double promedio = (totalSocios > 0) ? (sumaSaldo / totalSocios) : 0;

    //mostrar
    ui->lblTotalSociosCard->setText("Total Socios: " + QString::number(totalSocios));
    ui->lblSaldoTotalCard->setText("Saldo Total: L. " + QString::number(sumaSaldo, 'f', 2));
    ui->lblPromedioCard->setText("Promedio: L. " + QString::number(promedio, 'f', 2));
}

/**
 * @brief Carga datos seleccionados para edición
 */
void MainWindow::on_btnEditar_clicked()
{
    int fila = ui->tableSocios->currentRow();

    if(fila < 0){
        QMessageBox::warning(this, "Error", "Seleccione un registro");
        return;
    }

    //guardar id seleccionado
    idSeleccionado = ui->tableSocios->item(fila, 0)->text();

    //cargar datos de inputs
    QString nombre = ui->tableSocios->item(fila, 1)->text();
    double saldo = ui->tableSocios->item(fila, 2)->data(Qt::UserRole).toDouble();

    ui->txtNombre->setText(nombre);
    ui->txtSaldo->setText(QString::number(saldo));

    QMessageBox::information(this, "Editar", "Datos cargados, ahora edita y presiona Actualizar");
}

/**
 * @brief Guarda cambios del socio editado
 */
void MainWindow::on_btnActualizar_clicked()
{

    if(idSeleccionado.isEmpty()){
        QMessageBox::warning(this, "Error", "No has seleccionado");
        return;
    }

    QString nombre = ui->txtNombre->text().trimmed();
    QString saldoStr = ui->txtSaldo->text().trimmed();

    bool ok;
    double saldo = saldoStr.toDouble(&ok);

    if(nombre.isEmpty() || !ok){
        QMessageBox::warning(this, "Error", "Datos inválidos");
        return;
    }

    auto reply = QMessageBox::question(this, "Confirmar", "¿Guardar cambios?", QMessageBox::Yes | QMessageBox::No);

    if(reply == QMessageBox::No) return;

    auto datos = FileHandler::leerArchivo("socios.txt");

    SocioManager::actualizarSocio(datos, idSeleccionado.toStdString(), nombre.toStdString(), saldo);

    FileHandler::escribirArchivo("socios.txt", datos);

    LogManager::registrar("ACTUALIZADO: ID " + idSeleccionado.toStdString() + " -> " + nombre.toStdString() + " - L. " + std::to_string(saldo));

    cargarDatos();
    actualizarDashboard();

    ui->txtNombre->clear();
    ui->txtSaldo->clear();
    idSeleccionado = "";

    QMessageBox::information(this, "OK", "Actualizado");

}

/**
 * @brief Abre ventana de historial de acciones
 */
void MainWindow::on_btnVerLogs_clicked()
{

    LogViewer viewer(this);
    viewer.exec();
}

/*

void MainWindow::mostrarGrafico()
{

    auto datos = FileHandler::leerArchivo("socios.txt");

    QBarSeries *series = new QBarSeries();
    QBarSet *set = new QBarSet("Saldos");

    QStringList categorias;

    for(const auto& linea : datos){

        size_t pos1 = linea.find(",");
        size_t pos2 = linea.find(",", pos1 + 1);

        if(pos1 == std::string::npos || pos2 == std::string::npos) continue;

        std::string nombre = linea.substr(pos1 + 1, pos2 - pos1 - 1);
        std::string saldoStr = linea.substr(pos2 + 1);

        double saldo = 0;
        try{
            saldo = std::stod(saldoStr);
        }catch(...){}

        *set << saldo;
        categorias << QString::fromStdString(nombre);
    }

    series->append(set);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Saldos por socio");

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categorias);

    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    chartView->resize(600, 400);
    chartView->show();
}


void MainWindow::on_btnGrafico_clicked()
{
    mostrarGrafico();
}
*/

