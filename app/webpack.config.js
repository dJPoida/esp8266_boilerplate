const path = require('path');
const HtmlWebpackPlugin = require('html-webpack-plugin');
const CleanWebpackPlugin = require('clean-webpack-plugin');
const CopyWebpackPlugin = require('copy-webpack-plugin');
// @TODO: allow for easy renaming of the 'arduino_project' directory via package or env or something
const arduinoDataPath = path.join(__dirname, '../arduino_project/data');
const env = process.env.NODE_ENV;
const titleAPMode = "ESP8266 Boilerplate Config";
const titleIndex = "ESP8266 Boilerplate";

module.exports = {
    mode: env || 'development',
    entry: {
        // Access Point Mode Config Page
        apmode: path.join(__dirname, '/src/apmode.js'),

        // Normal Mode Control Page
        index: path.join(__dirname, '/src/index.js')
    },
    output: {
        path: arduinoDataPath,
        filename: '[name].js'
    },  
    module:{
        rules:[
            {
                test: /\.js$/,
                exclude: /node_modules/,
                loader: 'babel-loader'
            },
            {
                test:/\.css$/,
                use:['style-loader','css-loader']
            }
        ]
    },
    plugins:[
        // Clean up our arduino_project/data folder
        new CleanWebpackPlugin(
            [arduinoDataPath],
            {
                allowExternal: true
            }
        ),

        // Access Point Mode Config Page
        new HtmlWebpackPlugin(
            {
                title: titleAPMode,
                filename: "apmode.html",
                template: path.join(__dirname, '/src/apmode.html')
            }
        ),

        // Normal Mode Control Page
        new HtmlWebpackPlugin(
            {
                title: titleIndex,
                filename: "index.html",
                template: path.join(__dirname, '/src/index.html')
            }
        ),

        // Copy other static assets to our dist folder
        new CopyWebpackPlugin([
            
            // Compiled SCSS (css)
            {
                from: 'pub/css',
                to: path.join(arduinoDataPath, '/css/')
            }
        ]),   
    ],

    // Webpack Development Server
    devServer: {
        index: "index.html",
        contentBase: path.join(arduinoDataPath, '/'),
        compress: true,
        port: 8080,
        watchContentBase: true
    }
}