from PyQt5.QtCore import *
from PyQt5.QtSql import QSqlQuery, QSqlQueryModel


class RecordModel(QSqlQueryModel):
    def __init__(self, parent):
        super().__init__(parent=parent)

        self._roles = {
            0: self.tr("Record ID"),
            1: self.tr("Name"),
            2: self.tr("Surname"),
            3: self.tr("Last Modified"),
            4: self.tr("Created"),
        }

    def headerData(self, section, orientation, role):
        if orientation == Qt.Orientation.Vertical:
            return super().headerData(section, orientation, role)

        if role == Qt.DisplayRole:
            return self._roles[section]

        return QVariant()

    def filterResults(self, record_id, name, surname, startDate, endDate):
        baseQuery = "SELECT record_id, name, surname, \
            strftime('%Y-%m-%d %H:%M', last_modified), strftime('%Y-%m-%d %H:%M', created) \
            FROM Records \
            {whereClause} \
            LIMIT 100"

        startDate = startDate.toString("yyyy-MM-dd hh:mm:ss:zzz")
        endDate = endDate.toString("yyyy-MM-dd hh:mm:ss:zzz")
        whereClause = f"WHERE INSTR(record_id, '{record_id}')>0 AND INSTR(name, :name)>0 AND INSTR(surname, :surname)>0 "
        whereClause += f"AND last_modified BETWEEN '{startDate}' AND '{endDate}' "

        query = QSqlQuery()
        query.prepare(baseQuery.format(whereClause=whereClause))
        query.bindValue(":name", name)
        query.bindValue(":surname", surname)
        query.exec()

        self.setQuery(query)
