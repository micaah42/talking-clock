import {NgModule} from '@angular/core';
import {RouterModule, Routes} from '@angular/router';
import {OverviewComponent} from "./overview/overview.component";
import {AlarmsComponent} from "./alarms/alarms.component";
import {DevToolsComponent} from "./dev-tools/dev-tools.component"; // CLI imports router

const routes: Routes = [
  {path: '', component: OverviewComponent},
  {path: 'alarms', component: AlarmsComponent},
  {path: 'devtools', component: DevToolsComponent}
];

// configures NgModule imports and exports
@NgModule({
  imports: [RouterModule.forRoot(routes)],
  exports: [RouterModule]
})
export class AppRoutingModule {
}
