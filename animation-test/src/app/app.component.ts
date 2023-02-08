import { Component, OnInit } from '@angular/core';
import { interpolate, Shape } from 'flubber';

import { routes } from './app-routing.module';

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.scss']
})
export class AppComponent implements OnInit {
  routes = routes;
  ngOnInit() {
    console.log('routes', this.routes)
  }
}
