#include "NodeModel.h"

static const char s_treeNodeMimeType[] = "application/x-transform";

NodeModel::NodeModel(QObject *parent)
	: QAbstractItemModel(parent), 
	RootNode(nullptr)
{
}

NodeModel::~NodeModel()
{
}

Node& NodeModel::getItem(const QModelIndex &index) const
{
	if (index.isValid()) {
		Node *node = static_cast<Node*>(index.internalPointer());
		if (node)
			return *node;
	}
	return *RootNode;
}

QModelIndex NodeModel::index(int row, int column, const QModelIndex &parent) const
{
	if (parent.isValid() && parent.column() != 0)
		return QModelIndex();
	
	Node &parentItem = getItem(parent);

	Node &childItem = parentItem.child(row);
	if (&childItem)
		return createIndex(row, column, &childItem);
	else
		return QModelIndex();
}

QModelIndex NodeModel::parent(const QModelIndex &index) const
{
	if (!index.isValid())
		return QModelIndex();

	Node &childNode = getItem(index);
	Node &parentNode = childNode.parent();

	if (&parentNode == RootNode)
		return QModelIndex();

	return createIndex(parentNode.childNumber(), 0, &parentNode);
}

QModelIndex NodeModel::FindIndexByName(const std::string& objectname)
{
	void* returnValue = nullptr;
	RootNode->ExecuteReturn_B(&FindByName, returnValue, static_cast<const void*>(&objectname));
	Node* node = nullptr;
	if (returnValue)
	{
		node = (Node*)returnValue;
		const int row = node->childNumber();
		return createIndex(row, 0, node);
		
	}
	return QModelIndex();
}

bool NodeModel::insertEmptyRow(int position, const QModelIndex &parent)
{
	Node& parentNode = getItem(parent);

	beginInsertRows(parent, position, position + 1);
	endInsertRows();

	return true;
}

bool NodeModel::insertRows(int position, int rows, const QModelIndex &parent)
{
	Node& parentNode = getItem(parent);
	bool success;

	beginInsertRows(parent, position, position + rows - 1);
	success = parentNode.insertChildren(position, rows, 1);
	endInsertRows();

	return success;
}

bool NodeModel::removeRows(int position, int rows, const QModelIndex &parent)
{
	Node &parentNode = getItem(parent);
	bool success = true;

	beginRemoveRows(parent, position, position + rows - 1);
	success = parentNode.removeChild(position);
	endRemoveRows();

	return success;
}

void NodeModel::rejectNode(Node &node)
{
	const int row = node.childNumber();
	QModelIndex idx = createIndex(row, 0, &node);
	beginRemoveRows(idx.parent(), row, row);
	node.parent().RejectNode(row);
	endRemoveRows();
}

void NodeModel::removeNode(Node &node)
{
	const int row = node.childNumber();
	QModelIndex idx = createIndex(row, 0, &node);
	beginRemoveRows(idx.parent(), row, row);
	node.parent().removeChild(row);
	endRemoveRows();
}

void NodeModel::removeNode(const QModelIndex &index)
{
	Node &node = getItem(index);
	removeNode(node);
}

bool NodeModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
	Q_ASSERT(action == Qt::MoveAction);
	Q_UNUSED(column);

	if (!data->hasFormat(s_treeNodeMimeType))
		return false;


	QByteArray encodedData = data->data(s_treeNodeMimeType);
	QDataStream stream(&encodedData, QIODevice::ReadOnly);

	qint64 senderPid;
	stream >> senderPid;
	if (senderPid != QCoreApplication::applicationPid()) {
		// Let's not cast pointers that come from another process...
		return false;
	}

	Node &destNode = getItem(parent);
	Q_ASSERT(&destNode);


	int count;
	stream >> count;
	if (row == -1) {
		// valid index means: drop onto item. I chose that this should insert
		// a child item, because this is the only way to create the first child of an item...
		// This explains why Qt calls it parent: unless you just support replacing, this
		// is really the future parent of the dropped items.
		if (parent.isValid())
			row = 0;
		else
			// invalid index means: append at bottom, after last toplevel
			row = rowCount(parent);
	}

	// Move
	for (int i = 0; i < count; ++i) {
		// Decode data from the QMimeData
		qlonglong nodePtr;
		stream >> nodePtr;
		Node* sourceNode = reinterpret_cast<Node*>(nodePtr);

		if (sourceNode->childNumber() < row && &destNode == &sourceNode->parent())
			--row;

		// Remove from old position
		rejectNode(*sourceNode);

		beginInsertRows(parent, row, row);
		destNode.insertChild(row, *sourceNode);
		endInsertRows();
		
		++row;
	}
	return true;
}

QMimeData * NodeModel::mimeData(const QModelIndexList & indexes) const
{
	QMimeData *mimeData = new QMimeData();
	QByteArray encodedData;

	QDataStream stream(&encodedData, QIODevice::WriteOnly);
	QList<Node*> nodes;

	foreach(QModelIndex index, indexes) {
		if (index.isValid()) {
			
			Node &node = getItem(index);
			if (!nodes.contains(&node))
			{
				nodes << &node;
			}
		}
	}

	stream << QCoreApplication::applicationPid();
	stream << nodes.count();
	
	foreach(Node *node, nodes) {
		stream << reinterpret_cast<qlonglong>(node);
	}

	mimeData->setData(s_treeNodeMimeType, encodedData);
	return mimeData;
}

QStringList NodeModel::mimeTypes() const
{
	QStringList types;
	types << s_treeNodeMimeType;
	return types;
}

int NodeModel::rowCount(const QModelIndex &parent) const
{
	Node &parentNode = getItem(parent);

	return parentNode.childCount();
}

int NodeModel::columnCount(const QModelIndex & /* parent */) const
{
	return 1;
}

QVariant NodeModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	if (role != Qt::DisplayRole && role != Qt::EditRole)
		return QVariant();

	Node &node = getItem(index);
	QString Node = node.data().c_str();

	return QVariant(Node);
}

bool NodeModel::setData(QModelIndex &index, Object& object)
{
	Node &node = getItem(index);
	node.SetNodeObject(object);

	setData(index, QVariant(object.Name.c_str()), Qt::EditRole);
	return true;
}

bool NodeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if (role != Qt::EditRole)
		return false;

	emit dataChanged(index, index);

	return true;
}

bool NodeModel::setHeaderData(int section, Qt::Orientation orientation,
	const QVariant &value, int role)
{
	QString strValue = value.toString();
	Object& object = RootNode->GetNodeObject();
	object.Name = strValue.toUtf8().constData();

	emit headerDataChanged(orientation, section, section);

	return true;
}

QVariant NodeModel::headerData(int section, Qt::Orientation orientation,
	int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
	{
		QString Node = RootNode->data().c_str();
		return QVariant(Node);
	}

	return QVariant();
}

Qt::ItemFlags NodeModel::flags(const QModelIndex &index) const
{
	Qt::ItemFlags defaultFlags = QAbstractItemModel::flags(index);

	if (index.isValid())
		return Qt::ItemIsEditable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | defaultFlags;
	else
		return Qt::ItemIsEditable | Qt::ItemIsDropEnabled | defaultFlags;
}

Qt::DropActions NodeModel::supportedDropActions() const
{
	return Qt::MoveAction;
}

