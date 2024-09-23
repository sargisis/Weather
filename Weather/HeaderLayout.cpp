#include "HeaderLayout.h"

HeaderLayout::HeaderLayout(QWidget* parent)
    : QHBoxLayout(parent)
{
    m_header_label = new QLabel("Header Layout", parent);
    m_header_label->setStyleSheet("background-color: blue; color: white;");
    addWidget(m_header_label);
}


void HeaderLayout::createInputDialogInHeaderLayout()
{
    bool ok;
    QString text = QInputDialog::getText(nullptr, QObject::tr("Input Dialog"),
                                         QObject::tr("Enter some text:"), QLineEdit::Normal,
                                         "", &ok);
    if (ok && !text.isEmpty()) {
        m_header_label->setText(text);
    }
}
