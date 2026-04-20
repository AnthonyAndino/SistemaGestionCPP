#include "LogViewer.h"
#include <QTextEdit>
#include <QVBoxLayout>
#include <fstream>
#include <QTextCursor>
#include <QPushButton>

/**
 * @brief Ventana que muestra el historial de logs del sistema.
 */
LogViewer::LogViewer(QWidget *parent) : QDialog(parent){

    setWindowTitle("Historial de cambios");
    resize(500, 400);

    textEdit = new QTextEdit(this);
    textEdit->setReadOnly(true);

    textEdit->setStyleSheet(R"(
        background-color: #1e1e1e;
        color: #00ffcc;
        font-family: Consolas;
    )");

    QPushButton *btnRefresh = new QPushButton("Actualizar");

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(textEdit);
    layout->addWidget(btnRefresh);

    setLayout(layout);

    connect(btnRefresh, &QPushButton::clicked, this, &LogViewer::cargarLogs);

    cargarLogs();
}

/**
 * @brief Carga el contenido del archivo de logs en pantalla.
 */
void LogViewer::cargarLogs(){

    std::ifstream archivo("log.txt");
    std::string linea;

    QString contenido;

    // Leer archivo línea por línea
    while(getline(archivo, linea)){
        contenido += QString::fromStdString(linea) + "\n";
    }

    archivo.close();

    // Mostrar logs en el QTextEdit
    textEdit->setText(contenido);

    // Mover cursor al final del texto
    textEdit->moveCursor(QTextCursor::End);
}