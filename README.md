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
- [ ] getAllTemplate
- [ ] modifyTemplate
- [ ] addTemplate
- [ ] deleteTemplate
- [ ] addRecord
- [ ] getRecordList
- [ ] getRecordByID
- [ ] modifyRecordByID
- [ ] deleteRecordByID

### prescription:
- [ ] getPrescriptionList
- [ ] addPrescription
- [ ] deletePrescription
- [ ] getPrescriptionByID
- [ ] getPendingPrescriptionList
- [ ] handlePrescription
