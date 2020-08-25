const Node = require('./Node');
const Utils = require('./Utils');
const state = require('./Global').state;

class Layout extends Node {
    // Example Layout structure
    /* {
        "__type__": "cc.Layout",
        "_name": "",
        "_objFlags": 0,
        "node": {
            "__id__": 4
        },
        "_enabled": true,
        "_layoutSize": {
            "__type__": "cc.Size",
            "width": 200,
            "height": 150
        },
        "_resize": 0,
        "_N$layoutType": 0,
        "_N$padding": 0,
        "_N$cellSize": {
            "__type__": "cc.Size",
            "width": 40,
            "height": 40
        },
        "_N$startAxis": 0,
        "_N$paddingLeft": 0,
        "_N$paddingRight": 0,
        "_N$paddingTop": 0,
        "_N$paddingBottom": 0,
        "_N$spacingX": 0,
        "_N$spacingY": 0,
        "_N$verticalDirection": 1,
        "_N$horizontalDirection": 0
    } */
    constructor(data) {
        super(data);
        this._jsonNode.object_type = 'Layout';
    }

    parse_properties() {
        super.parse_properties();
        this._properties = {node: this._properties};

        let spr_component = Node.get_node_component_of_type(this._node_data, 'cc.Sprite');
         if (spr_component &&spr_component._enabled ){
             if (spr_component._spriteFrame){
                this._properties.backgroundVisible = spr_component._enabled;
                let path = Utils.get_sprite_frame_name_by_uuid(spr_component._spriteFrame.__uuid__);
                //path = 'creator/'+path;
                this._properties.spriteFrameName = path
            }
         }
        

        let lay_component = Node.get_node_component_of_type(this._node_data, 'cc.Layout');
        this._properties.layoutType = lay_component._N$layoutType;
        this._properties.resizeMode = lay_component._resize;
        this._properties.paddingTop = lay_component._N$paddingTop
        this._properties.paddingBottom = lay_component._N$paddingBottom
        this._properties.paddingLeft = lay_component._N$paddingLeft
        this._properties.paddingRight = lay_component._N$paddingRight
        this._properties.spacingX = lay_component._N$spacingX
        this._properties.spacingY = lay_component._N$spacingY
        this._properties.startAxis = lay_component._N$startAxis
        this._properties.horizontalDirection = lay_component._N$horizontalDirection
        this._properties.verticalDirection = lay_component._N$verticalDirection

        this._properties.cellSize = {w:lay_component._N$cellSize.width,h:lay_component._N$cellSize.height}
    

    }
}

module.exports = Layout;
