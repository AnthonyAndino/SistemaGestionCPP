#ifndef LOGVIEWER_H
#define LOGVIEWER_H

#include <QDialog>

class QTextEdit;

/**
 * @brief Ventana para visualizar los logs del sistema.
 */
class LogViewer : public QDialog{

    Q_OBJECT

public:
    /**
     * @brief Constructor de la ventana de logs.
     */
    explicit LogViewer(QWidget *parent = nullptr);

private:
    QTextEdit *textEdit;

    /**
     * @brief Carga y muestra los logs en la interfaz.
     */
    void cargarLogs();
};

#endif
