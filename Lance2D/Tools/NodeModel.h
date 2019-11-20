#pragma once

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <QtWidgets>
#include <QMimeData>

#include <list>
#include <vector>

#include <Core/Node.h>
#include <GLObject/Object.h>


class NodeModel : public QAbstractItemModel
{
	Q_OBJECT

public:
	NodeModel(QObject* parent = nullptr);
	~NodeModel();

	Node *RootNode;

	QModelIndex FindIndexByName(const std::string& objectname);

	QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;

	QModelIndex parent(const QModelIndex &index) const override;

	int rowCount(const QModelIndex &parent = QModelIndex()) const override;

	int columnCount(const QModelIndex &parent = QModelIndex()) const override;

	QVariant data(const QModelIndex &index, int role) const override;

	bool setData(const QModelIndex &index, const QVariant &value,
		int role = Qt::EditRole) override;
	
	bool setData(QModelIndex &index, Object& object);

	bool setHeaderData(int section, Qt::Orientation orientation,
		const QVariant &value, int role = Qt::EditRole) override;

	QVariant headerData(int section, Qt::Orientation orientation,
		int role = Qt::DisplayRole) const override;

	bool insertEmptyRow(int position, const QModelIndex &parent);

	bool insertRows(int position, int rows,
		const QModelIndex &parent = QModelIndex()) override;

	bool removeRows(int position, int rows,
		const QModelIndex &parent = QModelIndex()) override;

	/*bool moveRows(const QModelIndex &sourceParent, int sourceRow, int count,
		const QModelIndex &destinationParent, int destinationChild) override;*/

	bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) override;

	Qt::ItemFlags flags(const QModelIndex &index) const override;

	Qt::DropActions	supportedDropActions() const override;

	QMimeData *mimeData(const QModelIndexList &indexes) const override;
	QStringList mimeTypes() const override;

	void rejectNode(Node &node);
	void removeNode(Node &node);
	void removeNode(const QModelIndex &index);

//private:
	Node& getItem(const QModelIndex &index) const;
};

