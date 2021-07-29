# MIS-Backend
MIS的服务器后端。medicine模块使用C、Glib和JSON-Glib作为CGI程序实现，record和
prescription模块使用Racket作为CGI程序实现。

## 后端实现进度

### medicine:
- [x] getAllMedicine
- [x] getHistoryByID
- [x] getHistoryOverview
- [x] increaseMedicine
- [x] modifyMedicine
- [x] addMedicine

### record:
- [x] getAllTemplate
- [x] modifyTemplate
- [x] addTemplate
- [x] deleteTemplate
- [x] addRecord
- [x] getRecordList
- [x] getRecordByID
- [x] modifyRecordByID
- [x] deleteRecordByID

### prescription:
- [x] getPrescriptionList
- [x] addPrescription
- [x] deletePrescription
- [x] getPrescriptionByID
- [x] getPendingPrescriptionList
- [x] handlePrescription
