const path = require('path');
const HtmlWebpackPlugin = require('html-webpack-plugin');

// @TODO: allow for easy renaming of the 'arduino_project' directory via package or env or something
const arduinoDataPath = path.join(__dirname, '../arduino_project/data');

module.exports = {
    entry: path.join(__dirname, '/src/index.js'),
    output: {
        path: arduinoDataPath,
        filename: 'build.js',
    },  
    module:{
        rules:[{
            test: /\.js$/,
            exclude: /node_modules/,
            loader: 'babel-loader'
        }]
    },
    plugins:[
        new HtmlWebpackPlugin(
            {template: path.join(__dirname, '/src/index.html')}
        )
    ],
    devServer: {
        index: "index.html",
        contentBase: arduinoDataPath,
        compress: true,
        port: 8080,
        watchContentBase: true,
        open: true,
        hot: true
    }
}