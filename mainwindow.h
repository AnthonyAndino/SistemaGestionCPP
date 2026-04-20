#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "SocioManager.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:

    /// Guarda un nuevo socio
    void on_btnGuardar_clicked();

    /// Actualiza los datos del dashboard
    void actualizarDashboard();

    /// Exporta los datos a CSV
    void on_btnExportar_clicked();

    /// Elimina el socio seleccionado
    void on_btnEliminar_clicked();

    /// Filtra los socios según búsqueda
    void on_txtBuscar_textChanged(const QString &arg1);

    /// Carga datos para edición
    void on_btnEditar_clicked();

    /// Aplica cambios a un socio
    void on_btnActualizar_clicked();

    /// Abre la ventana de logs
    void on_btnVerLogs_clicked();


private:
    Ui::MainWindow *ui;

    bool cargandoDatos = false;

    /// Carga los datos desde archivo a la tabla
    void cargarDatos();

    /// ID del socio seleccionado para edición
    QString idSeleccionado = "";


};
#endif // MAINWINDOW_H
